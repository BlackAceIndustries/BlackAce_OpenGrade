#include <Arduino.h>

/*
  OG GradeControl
*/
// Last change: 2021-11-12 by BlackAce 12h30
// to be used with OpenGrade3D v3.0.xx
// test with blade offset
// Only MCP4725 output 0-5V
//-------------------------------------------------------------------------
//to change the deadband go a the end a change
//the value in the cutval digital.write
//the original code is BlinkTheRelay from BrianTee AGopenGPS
//-------------------------------------------------------------------------


// Function STUBS for Platform IO
void SetOutput();
void ReadFromEEPROM();
void SaveToEEPROM();


// configured to Run on Esp 32

#include "EEPROM.h"
#include <SPI.h>
#include <Adafruit_MCP4725.h>
#include <Wire.h>
#include <Button.h>

///////////////////PID constants/////////////////////////
float kp=8; //Mine was 8
float ki=0.2; //Mine was 0.2
float kd=3100; //Mine was 3100
float delta_setpoint = 21;           //Should be the distance from sensor to the middle of the bar in mm
float PID_p, PID_i, PID_d, PID_total;
//////////////////////////////////////////////////////////

///////////////////////Inputs/outputs///////////////////////
//#define Analog_in = A0;
#define DAC1_ENABLE 4      // DAC 1 enable/
#define DAC2_ENABLE 6     //  DAC 2 enable/
// BTN's
#define OFF_INC_BTN 8      //signal to move the blade offset up 1 mm
#define OFF_DEC_BTN 6       //signal to move the blade offset down 1 mm
#define AUTO_BTN 7        // Auto Grade Activation Btn
// I2C
#define SCL_PIN 22      // I2C SCL PIN
#define SDA_PIN 21      // I2C SCL PIN
// LED's
#define LED_DW 2      //DO2 led down (if used)
#define LED_UP 5       //DO5 led up (if used)
#define LED_AUTO 9      //DO9 led auto
#define LED_ON A0       //A0 on led
////////////////////////////////////////////////////////////


////////////////////////Variables/////////////////////////
int Read = 0;
float distance = 0.0;
//float elapsedTime, currentTime, timePrev;        //Variables for time control
float delta_previous_error, delta_error;
int period = 50;  //Refresh rate period of the loop is 50ms

/// CNH Valve 
float remote1Val = .500; //set danfoss valve at middle
float remote2Val = .500;
bool danfoss = true; //set true for danfoss
double voltage = 0;
int analogOutput1 = 2048; //send to MCP4725
int analogOutput2 = 2048; //send to MCP4725


//loop time variables in milliseconds
const byte LOOP_TIME = 50; //20hz
unsigned long lastTime = LOOP_TIME;
unsigned long currentTime = LOOP_TIME; 

//Comm checks
byte watchdogTimer = 0;      //make sure we are talking to OpenGrade
byte serialResetTimer = 0;   //if serial buffer is getting full, empty it
bool settingsRecieved = false;

//EEPROM identifier
byte EEP_Ident = 138;

//Deadband setting
byte deadband = 5;

//Communication with OpenGrade
bool isDataFound = false, isSettingFound = false;
int header = 0, tempHeader = 0, temp;


//workSwitch
byte workSwitch = 1; //high is circuit open, low is switch grounded
byte autoEnable = 0;

// Cut Delta
float cutDelta = 0;
int bladeOffset = 0;

// Bools 
bool isAutoActive = false;
bool isOffsetActive = false;

/////////////////////////////////////////////////////////


///////////////////////Initalize Objects///////////////////////
// DAC's
Adafruit_MCP4725 Dac1;
Adafruit_MCP4725 Dac2;
// BTN's
Button offsetIncBtn(OFF_INC_BTN); 
Button offsetDecBtn(OFF_DEC_BTN);
Button autoEngageBtn (AUTO_BTN);

int cutValue = 0;

//AutoControl switch button  ***********************************************************************************************************
byte currentState = 1;
byte reading;
byte previous = 0;

//BladeOffset stuff ************************************************************
int bladeOffsetIn = 0, bladeOffsetOut = 0;

void setup()
{

  //set the baud rate
  Serial.begin(38400);

  if (danfoss) {
    Dac1.begin(0x60);
  }
  else {

    //DAC's
    pinMode(DAC1_ENABLE, OUTPUT);   
      //pinMode(DAC2_ENABLE. OUTPUT);    
    //LED's
    pinMode(LED_BUILTIN, OUTPUT);    
    pinMode(LED_DW, OUTPUT);
    pinMode(LED_UP, OUTPUT);
    pinMode(LED_AUTO, OUTPUT);
    pinMode(LED_ON, OUTPUT);
    //keep pulled high and drag low to activate, noise free safe
    offsetIncBtn.begin(); 
    offsetDecBtn.begin();
    autoEngageBtn.begin();  
  }
  ReadFromEEPROM();// read saved settings
}

void loop(){  //Loop triggers every 50 msec (20hz) and sends back offsets Pid ect

  currentTime = millis();
  //unsigned int time = currentTime;

  if (currentTime - lastTime >= LOOP_TIME)
  {
    lastTime = currentTime;

    //If connection lost to OpenGrade, the watchdog will count up
    if (watchdogTimer++ > 250) watchdogTimer = 32;

    //clean out serial buffer to prevent buffer overflow
    if (serialResetTimer++ > 20)
    {
      while (Serial.available() > 0) 
      { 
        //char t = Serial.read();
        serialResetTimer = 0;
      }
    }

    //safety - if Serial lost auto mode will deativate
    if (watchdogTimer > 30) isAutoActive = false;

    if (watchdogTimer < 29) {      
      //BladeOffset ************************************************
      if (offsetIncBtn.pressed()) bladeOffsetOut ++; 
      else if (offsetDecBtn.pressed()) bladeOffsetOut --;             
      

      //Set Voltage sent to OpAMP
      if (isAutoActive)      {
        SetOutput();
      }

      if (cutDelta < 0) {
        digitalWrite(LED_DW, HIGH); // lowering the blade
        digitalWrite(LED_UP, LOW);
      }
      if (cutDelta > 0) {
        digitalWrite(LED_UP, HIGH); // lift the blade
        digitalWrite(LED_DW, LOW);
      }
      if (cutDelta == 0) {
        digitalWrite(LED_UP, LOW);
        digitalWrite(LED_DW, LOW);
      }

    } //end of watchdog timed loop

    //****************************************************************************************
    //This runs continuously, outside of the timed loop, keeps checking UART for new data
    // PGN - 32762 - 127.250 0x7FFA
    //public int mdHeaderHi, mdHeaderLo = 1, cutValve = 2
    //Settind PGN - 32760 - 127.248 0x7FF8

    
    


    // if (Serial.available() > 0 && !isDataFound && !isSettingFound){
    //   int temp = Serial.read();
    //   header = tempHeader << 8 | temp;                //high,low bytes to make int
    //   tempHeader = temp;                              //save for next time
    //   if (header == 32762) isDataFound = true;        //Do we have a match?
    //   if (header == 32760) isSettingFound = true;        //Do we have a match?
    // }

    // //Data Header has been found, so the next 6 bytes are the data
    // if (Serial.available() > 5 && isDataFound){
    //   //pin(LED_BUILTIN) = HIGH;
    //   isDataFound = false;
    //   cutDelta = Serial.read(); // double - if below + if above 
    //   isAutoActive = Serial.read(); // 1 if Active
    //   bladeOffset = Serial.read(); //bladeOffset value in opengrade 100 mean 0 offset.
    //   Serial.read(); //optOut2
    //   Serial.read(); //optOut3
    //   Serial.read(); //optOut4

    //   //reset watchdog
    //   watchdogTimer = 0;

    //   //Reset serial Watchdog
    //   serialResetTimer = 0;

    //   //Print data to openGrade, MUST send 8 bytes!
    //   //New Data [cutDelta, isAutoActive, bladeOffsetOut, PGain, IGain, DGain, PID_Total]
            
    //   if (cutDelta < 0 && isAutoActive){    // lowering the blade
      
    //     Serial.print("1,");  // 
    //   }
    //   else if (cutDelta < 0 && isAutoActive){
    //     Serial.print("0,");
    //   }
    //   else Serial.print("-,");

    //   Serial.print(cutDelta); // Return Cut Delta to OG
    //   Serial.print(",");
    //   Serial.print(isAutoActive); // 1 if auto mode is active      
    //   Serial.print(",");
    //   Serial.print(bladeOffsetOut); // blade offset in mm 
    //   Serial.print(",");      
    //   Serial.print(kp); // P gain 
    //   Serial.print(",");
    //   Serial.print(ki); // I Gain
    //   Serial.print(",");
    //   Serial.print(kd); // D Gain
    //   Serial.print(",");
    //   Serial.print(PID_total); // PID Total
    //   //Serial.print("\n");

    //   Serial.flush();   // flush out buffer
     

    // }

    //Setting Header has been found, so the next 8 bytes are the data
    if (Serial.available() > 7 && isSettingFound)
    {
      isSettingFound = false;
      deadband = Serial.read();
      Serial.read();
      Serial.read();
      Serial.read();
      Serial.read();
      Serial.read();
      Serial.read();
      Serial.read();      

      SaveToEEPROM();
      settingsRecieved = true;
    }
  }
}

void SetOutput() {

  analogOutput1 = 0;

  digitalWrite(LED_BUILTIN, HIGH);// led on when automode

      
  distance = cutDelta;  
  delta_error = (delta_setpoint + bladeOffset) - distance;   
  
  // calculate the P errror
  PID_p = kp * delta_error;
  
  //float delta_diference = delta_error - delta_previous_error;
  
  // calculate the d error
  PID_d = kd*((delta_error - delta_previous_error)/period);

  //calculate the i error
  if(-3 < delta_error && delta_error < 3){
    PID_i = PID_i + (ki * delta_error);
  }
  else{
    PID_i = 0;
  }

  PID_total = PID_p + PID_i + PID_d;        
  
  analogOutput1 = map(PID_total, -150, 150, 0, 4096);
  
  voltage = (analogOutput1/4096) * 5;
  
  //if(PID_total < 20){PID_total = 20;}
  //if(PID_total > 160) {PID_total = 160; }       
  
  if (abs(cutDelta) > deadband){
    Dac1.setVoltage(analogOutput1, false);  
  }  
  delta_previous_error = delta_error;  
}


void SaveToEEPROM() {
  EEPROM.write(1, 1);
  EEPROM.write(3, 3);
  EEPROM.write(5, 5);
  EEPROM.write(7, 7);
  EEPROM.write(9, 9);
  EEPROM.write(11, 11);
  EEPROM.write(13, 13);
  EEPROM.write(15, deadband);
  EEPROM.write(17, EEP_Ident);
}

void ReadFromEEPROM() {
  int checkValue;
  checkValue = EEPROM.read(17);
  if (checkValue == EEP_Ident) {
    EEPROM.read(1);
    EEPROM.read(3);
    EEPROM.read(5);
    EEPROM.read(7);
    EEPROM.read(9);
    EEPROM.read(11);
    EEPROM.read(13);
    deadband = EEPROM.read(15);
  }
}
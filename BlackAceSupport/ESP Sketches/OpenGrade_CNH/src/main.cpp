
/*
  OG GradeControl
*/
// Last change: 2021-11-12 by BlackAce 12h30
// to be used with OpenGrade v3.0.xx
// test with blade offset
// Only MCP4725 output 0-5V
//-------------------------------------------------------------------------
//to change the deadband go a the end a change
//the value in the cutval digital.write
//
//-------------------------------------------------------------------------

// Function STUBS for Platform IO
void SetOutput();



// configured to Run on Esp 32
#include "EEPROM.h"
#include <SPI.h>
#include <Adafruit_MCP4725.h>
#include <Wire.h>
#include <Button.h>
#include <PID_v1.h>

//////////////SETTINGS////////////
float Kp=38; //Mine was 38
float Ki=.02; //Mine was 0.02
float Kd=2800; //Mine was 2800
float delta_setpoint = 0;  

byte b_Ki, b_Kp, b_Kd;


///////////////////PID/////////////////////////
float PID_p, PID_i, PID_d, PID_total;
//////////////////////////////////////////////////////////

///////////////////////Inputs/outputs///////////////////////
//#define Analog_in = A0;
#define DAC1_ENABLE 4      // DAC 1 enable/
#define DAC2_ENABLE 5     //  DAC 2 enable/
// BTN's
#define OFF_INC_BTN 2      //signal to move the blade offset up 1 mm
#define OFF_DEC_BTN 0       //signal to move the blade offset down 1 mm
#define AUTO_BTN 15        // Auto Grade Activation Btn
// I2C
#define SCL_PIN 22      // I2C SCL PIN
#define SDA_PIN 21      // I2C SCL PIN
// Debug
#define RXD2 16
#define TXD2 17
//
#define VALVE_FLOAT 2048

// LED's
//#define LED_DW 2      //DO2 led down (if used)
//#define LED_UP 5       //DO5 led up (if used)
//#define LED_AUTO 9      //DO9 led auto
//#define LED_ON A0       //A0 on led
////////////////////////////////////////////////////////////


////////////////////////Variables/////////////////////////
float delta_previous_error, delta_error;

/// CNH Valve 
bool danfoss = true; //set true for danfoss
int analogOutput1 = 2048; //send to MCP4725
int analogOutput2 = 2048; //send to MCP4725
double voltage = 0; // diagnostic Voltage

//loop time variables in milliseconds
const byte LOOP_TIME = 50; //20hz
const byte LOOP_TIME2 = 5;
unsigned long lastTime = LOOP_TIME;
unsigned long lastTime2 = LOOP_TIME2; 
unsigned long currentTime = LOOP_TIME; 

//Define Variables we'll be connecting to
//double Setpoint, Input, Output;  // FOR pid Library

//Comm checks
byte watchdogTimer = 0;      //make sure we are talking to OpenGrade
byte serialResetTimer = 0;   //if serial buffer is getting full, empty it
bool settingsRecieved = false;

//EEPROM identifier
byte EEP_Ident = 138;

//Deadband setting
byte deadband = 2;

//Communication with OpenGrade
bool isDataFound = false, isSettingFound = false;
int header = 0, tempHeader = 0, temp;

// Cut Delta


// Bools 
bool isAutoActive = false;
bool isCutting = false;
byte b_autoState = 0;
byte b_deltaDir = 0;
byte b_cutDelta = 0;
int cut1 = -1;

/////////////////////////////////////////////////////////


///////////////////////Initalize Objects///////////////////////
// DAC's
Adafruit_MCP4725 Dac1;
Adafruit_MCP4725 Dac2;
// BTN's
Button offsetIncBtn(OFF_INC_BTN); 
Button offsetDecBtn(OFF_DEC_BTN);
Button autoEngageBtn (AUTO_BTN);

// PID Controller
//PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

//BladeOffset stuff ************************************************************
int bladeOffsetOut = 0;


void setup()
{
  //set the baud rate
  Serial.begin(38400);  
  
  pinMode(2, OUTPUT);
  
  digitalWrite(2, HIGH); delay(500); digitalWrite(2, LOW); delay(500); digitalWrite(2, HIGH); delay(500);
  digitalWrite(2, LOW); delay(500); digitalWrite(2, HIGH); delay(500); digitalWrite(2, LOW); delay(500);
  

  offsetIncBtn.begin(); 
  offsetDecBtn.begin();
  autoEngageBtn.begin();

  digitalWrite(2, HIGH);
  delay(2000);
  digitalWrite(2, LOW);
  
  Dac1.begin(0x62);
  
}

void loop(){  //Loop triggers every 50 msec (20hz) and sends back offsets Pid ect

  currentTime = millis();
  
  if (currentTime - lastTime >= LOOP_TIME)
  {    
    lastTime = currentTime;
    
    if (b_autoState == 1){
      isAutoActive = true;
    }
    else {
      isAutoActive = false;
    }     

    //////BTNS    =============================================== 
    if (offsetIncBtn.pressed()){
      bladeOffsetOut ++; 
    } 
    if (offsetDecBtn.pressed()){ 
      bladeOffsetOut --;
    }
    if (autoEngageBtn.pressed()){
      if (isAutoActive){
        isAutoActive = false;
        b_autoState = 0;
      }
      else {
        isAutoActive = true;
        b_autoState = 1;
      }
    } 

    Serial.print(bladeOffsetOut);
    Serial.print(", ");
    Serial.print(b_autoState);
    Serial.print(", ");
    Serial.println(voltage);           
  }
  
  ///
  ///Serial Loop
  ///
  if (currentTime - lastTime2 >= LOOP_TIME2){

    if (Serial.available() > 0 && !isDataFound && !isSettingFound)
    {
      //Serial1.println("Serial Available Data and settings not found!");
      int temp = Serial.read();    
      header = tempHeader << 8 | temp;                //high,low bytes to make int
      tempHeader = temp;  
      //Serial1.println(header);                            //save for next time
      if (header == 32762) isDataFound = true;        //Do we have a match?
      if (header == 32760) isSettingFound = true;        //Do we have a match?
    }

    //Data Header has been found, so the next 6 bytes are the data
    if (Serial.available() > 3 && isDataFound)   //
    {     
      
      isDataFound = false;      
      b_deltaDir = Serial.read();// Cut Delta Dir 
      b_cutDelta = Serial.read();// Cut Delta      
      b_autoState = Serial.read();// Auto State
      
      if (b_deltaDir == 3){
        isCutting = false;
      }
      else{
        isCutting = true;
      }           
    }
    
    //Setting Header has been found, so the next 8 bytes are the data
    if (Serial.available() > 7 && isSettingFound)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      isSettingFound = false;
      b_Kp = Serial.read();
      b_Ki = Serial.read();
      b_Kd = Serial.read();
      
      Kp = double(b_Kp);
      Ki = double(b_Ki / 100);
      Kd = double(b_Kp * 100);
    }
    
    if (isAutoActive && isCutting){
      SetOutput();
    }
    else{
      analogOutput1 = VALVE_FLOAT;
      voltage = ((double)analogOutput1/4096) * 5.0;
      Dac1.setVoltage(analogOutput1, false);                      
    }
  }
}

///
/// Functions
///

void SetOutput()
{
  
  analogOutput1 = 2048;  
  
  if (b_deltaDir == 0){
    cut1 = -(int)b_cutDelta;
  }  
  else {
    cut1 = (int)b_cutDelta;
  }

  delta_error = (delta_setpoint) - cut1;

  PID_p = Kp * delta_error;// calculate the P errror  
  
  PID_d = Kd*((delta_error - delta_previous_error)/LOOP_TIME);// calculate the d error
  
  if(-3 < delta_error && delta_error < 3){  // 3 cm deadband for i
    PID_i = PID_i + (Ki * delta_error);//calculate the i error
  }
  else{
    PID_i = 0;
  }

  PID_total = PID_p + PID_i + PID_d;

  if (PID_total >  2300) PID_total = 2300;      
  if (PID_total <  -2300) PID_total = -2300;

  if (b_deltaDir == 1){ // Delta is Positive need to lower IMP
    analogOutput1 = map(PID_total, 0.0, -2300, 2048, 0);
  }

  else if (b_deltaDir == 0){// Delta is Negative need to raise IMP
    analogOutput1 = map(PID_total,  0.0, 2300, 2048, 4096);
  }
  
  if (analogOutput1 >= 4090) analogOutput1 = 4090; // do not exceed 4096
  if (analogOutput1 < 0) analogOutput1 = 2; // do not write negative numbers 

  voltage = ((double)analogOutput1/4096) * 5.0;   
  Dac1.setVoltage(analogOutput1, false);

  delta_previous_error = delta_error;  
}


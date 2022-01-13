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
/* Board layout:
            Front
         +----------+
         |         *| RST   PITCH  ROLL  HEADING
     ADR |*        *| SCL
     INT |*        *| SDA     ^            /->
     PS1 |*        *| GND     |            |
     PS0 |*        *| 3VO     Y    Z-->    \-X
         |         *| VIN
         +----------+
*/



// Function STUBS for Platform IO
bool SetupGradeController();
bool SetAutoState();
void CheckBtns();
void RecvSerialPortData();
void SendSerialPortData();
void SetOutput();
void SetValveLimits();

// configured to Run on Esp 32
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "EEPROM.h"
#include <SPI.h>
#include <Adafruit_MCP4725.h>
#include <Wire.h>
#include <Button.h>

///////////////////////PINS///////////////////////
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
#define CNH 0
#define DEERE 1
#define DANFOSS 2
#define CNH_MIN .11
#define CNH_MAX .89
#define DEERE_MIN .11
#define DEERE_MAX .89
#define DANFOSS_MIN .26
#define DANFOSS_MAX .74



//////////////SETTINGS////////////
float Kp=35; //Mine was 38
float Ki=.02; //Mine was 0.02
float Kd=3100; //Mine was 2800
float delta_setpoint = 0;  


///////////// Com Bytes///////////////////////
byte b_Ki, b_Kp, b_Kd;
byte b_autoState = 0, b_deltaDir = 0, b_cutDelta = 0;
byte b_bladeOffsetOut = 0;
byte b_retDeadband = 25;
byte b_extDeadband = 75;
byte b_valveType = 255;   // 0= CNH    1= Deere     2= Danfoss
byte b_deadband = 2;

///////////////////PID/////////////////////////
float PID_p, PID_i, PID_d, PID_total;
float delta_previous_error, delta_error;
//////////////////////////////////////////////////////////

/// CNH Valve 
uint16_t analogOutput1 = VALVE_FLOAT; //send to MCP4725
uint16_t analogOutput2 = VALVE_FLOAT; //send to MCP4725
int cut1 = -1;
double voltage = 0; // diagnostic Voltage
int retDeadband = 1845;
int extDeadband = 2250;
int retMin = (0.11 * 4096);   //450.56  CNH 
int extMax = (0.89 * 4096);   //3645
bool isAutoActive = false;
bool isCutting = false;

//loop time variables in milliseconds
const byte LOOP_TIME = 50; //20hz
const byte LOOP_TIME2 = 5;
unsigned long lastTime = LOOP_TIME;
unsigned long lastTime2 = LOOP_TIME2; 
unsigned long currentTime = LOOP_TIME; 

//Comm checks
byte watchdogTimer = 0;      //make sure we are talking to OpenGrade
byte serialResetTimer = 0;   //if serial buffer is getting full, empty it
bool settingsRecieved = false;

//Communication with OpenGrade
bool isDataFound = false, isSettingFound = false;
int header = 0, tempHeader = 0, temp;

///////////////////////Initalize Objects///////////////////////
// DAC's
Adafruit_MCP4725 Dac1;
Adafruit_MCP4725 Dac2;
// BTN's
Button offsetIncBtn(OFF_INC_BTN); 
Button offsetDecBtn(OFF_DEC_BTN);
Button autoEngageBtn (AUTO_BTN);

// Function STUBS for Platform IO
bool SetupGradeController();
bool SetAutoState();
void CheckBtns();
void RecvSerialPortData();
void SendSerialPortData();
void SetOutput();

void setup()
{
  SetupGradeController();
}

void loop(){  //Loop triggers every 50 msec (20hz) and sends back offsets Pid ect

  currentTime = millis();  
  if (currentTime - lastTime >= LOOP_TIME)
  {    
    lastTime = currentTime; 
    SendSerialPortData();      
  }
  
  if (currentTime - lastTime2 >= LOOP_TIME2){
    lastTime2 = currentTime;
    CheckBtns();
    SetAutoState(); 
    RecvSerialPortData();
  }
}

///
/// Functions
///


bool SetupGradeController()
{
  //set the baud rate
  Serial.begin(38400);  
  //Serial1.begin(38400, SERIAL_8N1, RXD2, TXD2); 

  digitalWrite(2, HIGH); delay(500); digitalWrite(2, LOW); delay(500); digitalWrite(2, HIGH); delay(500);
  digitalWrite(2, LOW); delay(500); digitalWrite(2, HIGH); delay(500); digitalWrite(2, LOW); delay(500);

  offsetIncBtn.begin(); 
  offsetDecBtn.begin();
  autoEngageBtn.begin();
  Dac1.begin(0x62);
  Dac2.begin(0x63);
  return true;

}

bool SetAutoState(){
  if (b_deltaDir == 3){
    isCutting = false;
  }
  else{
    isCutting = true;
  }
  
  if (b_autoState == 1){
    isAutoActive = true;
    return true;
  }
  else {
    isAutoActive = false;
    return false;
    
  }     
}

void CheckBtns(){ 
  //////BTNS    =============================================== 
  if (offsetIncBtn.pressed()){
    b_bladeOffsetOut ++; 
  } 
  if (offsetDecBtn.pressed()){ 
    b_bladeOffsetOut --;
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
}

void RecvSerialPortData(){
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

  //Data Header has been found, so the next x bytes are the data
  if (Serial.available() > 3 && isDataFound)   //
  {     
    
    isDataFound = false;      
    b_deltaDir = Serial.read();// Cut Delta Dir 
    b_cutDelta = Serial.read();// Cut Delta      
    b_autoState = Serial.read();// Auto State     
    
    
  }
  
  //Setting Header has been found, so the next 8 bytes are the data
  if (Serial.available() > 9 && isSettingFound)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    isSettingFound = false;
    b_Kp = Serial.read();
    b_Ki = Serial.read();
    b_Kd = Serial.read();
    b_retDeadband = Serial.read();
    b_extDeadband = Serial.read();
    b_valveType = Serial.read();
    
    Kp = double(b_Kp);
    Ki = double(b_Ki / 100);
    Kd = double(b_Kp * 100);    
    SetValveLimits();     

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

void SendSerialPortData()
{    
  Serial.print(b_bladeOffsetOut);
  Serial.print(", ");
  Serial.print(b_autoState);
  Serial.print(", ");
  Serial.println(voltage);
}

void SetOutput()
{
  
  analogOutput1 = VALVE_FLOAT;  
  
  if (b_deltaDir == 0){
    cut1 = -(int)b_cutDelta;
  }  
  else {
    cut1 = (int)b_cutDelta;
  }

  delta_error = (delta_setpoint) - cut1;

  PID_p = Kp * delta_error;// calculate the P errror  
  
  PID_d = Kd*((delta_error - delta_previous_error)/LOOP_TIME);// calculate the d error
  
  if(-b_deadband < delta_error && delta_error < b_deadband){  // 3 cm deadband for i
    PID_i = PID_i + (Ki * delta_error);//calculate the i error
  }
  else{
    PID_i = 0;
  }

  PID_total = PID_p + PID_i + PID_d;

  if (PID_total >  2300) PID_total = 2300;      
  if (PID_total <  -2300) PID_total = -2300;

  if (b_deltaDir == 1){ // Delta is Positive need to lower IMP RETRACT
    analogOutput1 = map(PID_total, 0.0, -2300, retDeadband , retMin);
  }
  else if (b_deltaDir == 0){// Delta is Negative need to raise IMP
    analogOutput1 = map(PID_total,  0.0, 2300, extDeadband, extMax);
  }
  
  if (analogOutput1 >= extMax) analogOutput1 = extMax; // do not exceed 4096
  if (analogOutput1 <= retMin) analogOutput1 = retMin; // do not write negative numbers 
   
  
  if (b_cutDelta > b_deadband){
    Dac1.setVoltage(analogOutput1, false);
  }
  else {
    analogOutput1 = VALVE_FLOAT;
    Dac1.setVoltage(analogOutput1, false);
  }
  
  voltage = ((double)analogOutput1/4096) * 5.0;

  delta_previous_error = delta_error;  
}

void SetValveLimits(){

  switch(b_valveType) {
  
    case CNH:
      retDeadband = VALVE_FLOAT - ((b_retDeadband/200.0)*4096);
      extDeadband = VALVE_FLOAT + ((b_extDeadband/200.0)*4096);    
      retMin = (CNH_MIN * 4096);
      extMax = (CNH_MAX * 4096);
      break;
      
    case DEERE:
      retDeadband = VALVE_FLOAT - ((b_retDeadband/200.0)*4096);
      extDeadband = VALVE_FLOAT + ((b_extDeadband/200.0)*4096);
      retMin = (DEERE_MIN * 4096);
      extMax = (DEERE_MIN * 4096);
      break;
      
    case DANFOSS:
      retDeadband = VALVE_FLOAT - ((b_retDeadband/200.0)*4096);
      extDeadband = VALVE_FLOAT + ((b_extDeadband/200.0)*4096);
      retMin = (DANFOSS_MIN * 4096);
      extMax = (DANFOSS_MAX * 4096);
      break;

    default:
      retDeadband = VALVE_FLOAT - ((b_retDeadband/200.0)*4096);
      extDeadband = VALVE_FLOAT + ((b_extDeadband/200.0)*4096);    
      retMin = (CNH_MIN * 4096);
      extMax = (CNH_MAX * 4096);
      
  }

  
  
}

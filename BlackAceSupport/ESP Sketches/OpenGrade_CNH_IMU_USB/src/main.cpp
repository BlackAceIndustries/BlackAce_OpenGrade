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
bool SetupIMU();
bool SetAutoState();
void CheckBtns();
void RecvSerialPortData();
void GetIMUData();
void GetGradeControlData();
void SendDataToPort();
void SetOutput();
void SetValveLimits();
void ClearSerialBuff();

// configured to Run on Esp 32
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "EEPROM.h"
#include <SPI.h>
#include <Adafruit_MCP4725.h>
#include <Wire.h>


///////////////////////PINS///////////////////////
#define SERIAL_BAUD 115200
#define DAC1_ENABLE 4      // DAC 1 enable/
#define DAC2_ENABLE 5     //  DAC 2 enable/
#define SCL_PIN 22      // I2C SCL PIN
#define SDA_PIN 21      // I2C SCL PIN
#define RXD2 16
#define TXD2 17
#define IMU_ID 1200
#define CONST_180_DIVIDED_BY_PI 57.2957795130823
#define BNO055_SAMPLERATE_DELAY_MS (95)
//

// Valve Definitions
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

/////////////IMU///////////////
uint8_t OG_data[] = {0,0,0,0,0};
int16_t dataSize = sizeof(OG_data);
float headingIMU = 0;
float rollIMU = 0;
float pitchIMU = 0;
int16_t bno08xHeading10x = 0;
int16_t bno08xRoll10x = 0;

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

/////////////// CNH Valve /////////////////////////
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
const byte LOOP_TIME = 50; //20hz  Send Serial Loop 
const byte LOOP_TIME2 = 5; //100HZ    Recv Serial Loop
const byte LOOP_TIME3 = BNO055_SAMPLERATE_DELAY_MS; //10HZ  Poll IMU Loop

unsigned long lastTime = LOOP_TIME;
unsigned long lastTime2 = LOOP_TIME2;
unsigned long lastTime3 = LOOP_TIME3;  
unsigned long currentTime = 0; 


//Comm checks
byte watchdogTimer = 0;      //make sure we are talking to OpenGrade
byte serialResetTimer = 0;   //if serial buffer is getting full, empty it

//Communication with OpenGrade
bool isDataFound = false, isSettingFound = false;
int header = 0, tempHeader = 0, temp;

///////////////////////Initalize Objects///////////////////////
// IMU
TwoWire esp = TwoWire(5);
Adafruit_BNO055 bnoIMU = Adafruit_BNO055(IMU_ID, 0x28, &esp);  //
// DAC's
Adafruit_MCP4725 Dac1 = Adafruit_MCP4725();
Adafruit_MCP4725 Dac2 = Adafruit_MCP4725();


void setup()
{
  esp.begin(SDA_PIN , SCL_PIN);
  SetupGradeController();
  SetupIMU();  
  
  Dac1.setVoltage(VALVE_FLOAT, false);
  Dac2.setVoltage(VALVE_FLOAT, false);
  
}

void loop(){  //Loop triggers every 50 msec (20hz) and sends back offsets Pid ect

  currentTime = millis();  // Send GradeControl Data to OG
  SetOutput();
  SetAutoState();

  if (currentTime - lastTime >= LOOP_TIME)
  {  
    lastTime = currentTime;
    SendDataToPort();
                  
  }
  
  if (currentTime - lastTime2 >= LOOP_TIME2){ // Recv Data from OG 
    lastTime2 = currentTime;
    RecvSerialPortData();
  }
  
  if (currentTime - lastTime3 >= LOOP_TIME3){ //Poll Imu for data
   
    GetIMUData();
  }
}

///
/// Functions
///


bool SetupGradeController()
{
  //set the baud rate
  Serial.begin(SERIAL_BAUD);  
  Serial1.begin(115200, SERIAL_8N1, RXD2, TXD2); 

  digitalWrite(2, HIGH); delay(500); digitalWrite(2, LOW); delay(500); digitalWrite(2, HIGH); delay(500);
  digitalWrite(2, LOW); delay(500); digitalWrite(2, HIGH); delay(500); digitalWrite(2, LOW); delay(500);

  
  Dac1.begin(0x62, &esp);
  Dac2.begin(0x63, &esp); 

  return true;

}

bool SetupIMU()
{  
  if(!bnoIMU.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    //Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }   
   
  /* Use external crystal for better accuracy */
  bnoIMU.setExtCrystalUse(true);
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

void GetIMUData()
{
  lastTime3 = currentTime;
  /* Get a new sensor event*/ 
  sensors_event_t event;
  bnoIMU.getEvent(&event);  
  
  headingIMU = (360 - (float)event.orientation.x); // YAW  
  pitchIMU = ((float)event.orientation.y); // PITCH
  rollIMU = ((float)event.orientation.z); // ROLL   
  
  
}

void RecvSerialPortData(){
  
  if (Serial.available() > 0 && !isDataFound && !isSettingFound)
  {
    
    int temp = Serial.read();    
    header = tempHeader << 8 | temp;                //high,low bytes to make int
    tempHeader = temp;      //save for next time
    if (header == 32762) {//Do we have a match?
      isDataFound = true; 
      //Serial1.print("Data Recieved   Bytes in Buffer -> ");
      //Serial1.println(Serial.available());   
    }    
    if (header == 32760){//Do we have a match?
      isSettingFound = true; 
      //Serial1.print("Settings Recieved   Bytes in Buffer -> ");
      //Serial1.println(Serial.available());
    }       
    
  }

  //Data Header has been found, so the next x bytes are the data
  if (Serial.available() > 2 && isDataFound  && !isSettingFound)   //
  {
    isDataFound = false;      
    b_deltaDir = Serial.read();// Cut Delta Dir         
    b_cutDelta = Serial.read();// Cut Delta        
    b_autoState = Serial.read();// Auto State 
    
    ClearSerialBuff();
  }
  
  //Setting Header has been found, so the next 8 bytes are the data
  if (Serial.available() > 6 && isSettingFound && !isDataFound)
  {
    //Serial1.println("Settings Found");
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

    ClearSerialBuff();
  }  
  
}

void SendDataToPort(){

  Serial.print(b_autoState);
  Serial.print(",");
  Serial.print(voltage);
  Serial.print(",");
  Serial.print((headingIMU));
  Serial.print(",");
  Serial.print(pitchIMU);
  Serial.print(",");
  Serial.println(rollIMU);
}

void SetOutput()
{

  if (isAutoActive && isCutting){    
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
    
    
    if (b_cutDelta < b_deadband){
      analogOutput1 = VALVE_FLOAT;
    }
    
    Dac1.setVoltage(analogOutput1, false);  
    voltage = ((double)analogOutput1/4096) * 5.0;
    delta_previous_error = delta_error;
  }
  else{
    analogOutput1 = VALVE_FLOAT;   
    Dac1.setVoltage(analogOutput1, false);
    voltage = ((double)analogOutput1/4096) * 5.0;                          
  }
  
    
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

void ClearSerialBuff(){
  while (Serial.available() > 0){  /// clean out serial buffer
        Serial.read();
    }
}

/*void GetGradeControlData()
{ 
  //OG_data[0] = (b_autoState);
  //b_autoState = 
  //OG_data[1] = (voltage);
  //voltage
}
*/
/*void CheckBtns(){ 
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
}*/

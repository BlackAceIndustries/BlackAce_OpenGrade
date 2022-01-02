#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Adafruit_MCP4725.h>
#include <Wire.h>
#include <Button.h>

///
/// UDP Variables
WiFiUDP Udp;  // Creation of wifi Udp instance

const char *ssid = "GradeControl";

char packetBuffer[255];
uint16_t localPort = 9999; //OpenGrade Server Port
uint16_t openGradePort = 7777; // OpenGrade Port
uint16_t portDestination = 9999; //AOG port that listens

IPAddress openGradeIP(192,168,0,120);   // Declaration of default IP for server
IPAddress gradeControlIP(127,0,0,1);   // Different IP than server (Client) this unit
IPAddress Subnet(255, 255, 255, 0);
IPAddress Dns(8,8,8,8);
//// ethernet mac address - must be unique on your network
//static uint8_t mymac[] = { 0x70,0x69,0x69,0x2A,0x30,0x31 };
//uint8_t data[] = {0x80,0x81,0x7D,0xD3,8, 0,0,0,0, 0,0,0,0, 15};
//int16_t dataSize = sizeof(data);

///
/// gradeControl Variables
///
////////////Inputs/outputs/////////////////////
#define DAC1_ENABLE 4      // DAC 1 enable/
#define DAC2_ENABLE 5     //  DAC 2 enable/
#define OFF_INC_BTN 2      //signal to move the blade offset up 1 mm
#define OFF_DEC_BTN 0       //signal to move the blade offset down 1 mm
#define AUTO_BTN 15        // Auto Grade Activation Btn
#define SCL_PIN 22      // I2C SCL PIN
#define SDA_PIN 21      // I2C SCL PIN
#define RXD2 16
#define TXD2 17
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

//////////////////PID/////////////////////
float Kp=38; //Mine was 8
float Ki=.02; //Mine was 0.2
float Kd=2800; //Mine was 3100
float delta_setpoint = 0; 
float PID_p, PID_i, PID_d, PID_total;
float delta_previous_error, delta_error;

//////////////////CNH Valve//////////////////// 
uint16_t analogOutput1 = VALVE_FLOAT; //send to MCP4725
uint16_t analogOutput2 = VALVE_FLOAT; //send to MCP4725
int cut1;
int bladeOffsetOut = 0;
int retDeadband = 1845;
int extDeadband = 2250;
int retMin = (0.11 * 4096);   //450.56  CNH 
int extMax = (0.89 * 4096);   //3645
double voltage; // voltage sent to OpenGrade
bool isAutoActive = false;
bool isCutting = false;

///
///Serial Variables
///
byte watchdogTimer = 0;      //make sure we are talking to OpenGrade
byte serialResetTimer = 0;   //if serial buffer is getting full, empty it
bool settingsRecieved = false;
bool isDataFound = false, isSettingFound = false; //Communication with OpenGrade
int header = 0, tempHeader = 0, temp; //Communication with OpenGrade

//ComBytes
byte b_Ki, b_Kp, b_Kd;
byte b_autoState = 0, b_deltaDir = 0, b_cutDelta = 0;
byte b_bladeOffsetOut = 0;
byte b_retDeadband = 25;
byte b_extDeadband = 75;
byte b_valveType = 255;   // 0= CNH    1= Deere     2= Danfoss
byte b_deadband = 2;

///
/// Timers
///
const byte LOOP_TIME = 50; //20hz  Serial Send loop
const byte LOOP_TIME2 = 10; //100hz  Serial Recv loop
unsigned long lastTime = LOOP_TIME;
unsigned long lastTime2 = LOOP_TIME2; 
unsigned long currentTime;

// Function STUBS for Platform IO
bool SetupUdp();
bool SetupGradeController();
bool SetAutoState();
void CheckBtns();
void RecvSerialPortData();
void SendSerialPortData();
void SetOutput();
bool SendUdpData();
bool RecvUdpData();
void SetValveLimits();

///////////////////////Initalize Objects///////////////////////

Adafruit_MCP4725 Dac1;  // DAC's
Adafruit_MCP4725 Dac2;
Button offsetIncBtn(OFF_INC_BTN); // BTN's
Button offsetDecBtn(OFF_DEC_BTN); 
Button autoEngageBtn (AUTO_BTN);

void setup() {
  
  SetupGradeController();
  SetupUdp();

}
  

void loop() {
  currentTime = millis(); 
  
  if (currentTime - lastTime >= LOOP_TIME)
  {
    lastTime = currentTime;        
    SendSerialPortData();
    
  }
  if (currentTime - lastTime2 >= LOOP_TIME2)
  {
    lastTime2 = currentTime;
    CheckBtns();
    SetAutoState();
    RecvSerialPortData(); 
  }
  //SendUdpData();
  //RecvUdpData(); 
}



bool SetupGradeController(){
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

bool SetupUdp(){  
  WiFi.begin(ssid);  
  WiFi.mode(WIFI_MODE_APSTA); // ESP-32 Server
  WiFi.config(gradeControlIP, gradeControlIP, Subnet, Dns);  
  Udp.begin(localPort);
  return true;
}

bool SendUdpData()
{
  //SENDING
    Udp.beginPacket(openGradeIP,9999);   //Initiate transmission of data
    
    Udp.printf("Millis: ");
    
    char buf[20];   // buffer to hold the string to append
    unsigned long testID = millis();   // time since ESP-32 is running millis() 
    sprintf(buf, "%lu", testID);  // appending the millis to create a char
    Udp.printf(buf);  // print the char
    
    Udp.printf("\r\n");   // End segment    
    Udp.endPacket();  // Close communication
    
    Serial.print("Time Since Startup: ");   // Serial monitor for user 
    Serial.println(buf);
    delay(5);
   // 
  return true;
}

bool RecvUdpData()
{
  //RECEPTION
  int packetSize = Udp.parsePacket();   // Size of packet to receive
  if (packetSize) {       // If we received a package
    
    int len = Udp.read(packetBuffer, sizeof(packetBuffer));
    
    if (len > 0) packetBuffer[len-1] = 0;
    Serial.print("RECIEVED(IP/Port/Size/Data): ");
    Serial.print(Udp.remoteIP());Serial.print(" / ");
    Serial.print(Udp.remotePort()); Serial.print(" / ");
    Serial.print(packetSize);Serial.print(" / ");
    Serial.println(packetBuffer);
    
  }
  Serial.println("");
  delay(5);
  return true;
  
}


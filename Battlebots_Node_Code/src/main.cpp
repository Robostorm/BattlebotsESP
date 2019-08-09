// Battlebots Controller Unit Code 
// by: John Brereton & Logan Greif
// Made for NodeMCU LoLin Board

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>
#include <Wire.h>
#include <EEPROM.h>
#define ATMEGA_I2C_ADDR 0x16



RF24 radio(D7, D8); // CE, CSN
//const byte addresses[][6] = {"00001", "00002"};

char sendPacket[32] = "";
char recievePacket[32] = "";

String controllerName;

int rightPower;
int leftPower;

void tlc_send(uint8_t channel, uint16_t value){
    uint8_t byte1 = channel + ((value & 0x000F) << 4);
    uint8_t byte2 = (value & 0x0FF0) >> 4;

    Wire.beginTransmission(ATMEGA_I2C_ADDR);
    Wire.write(byte1);
    Wire.write(byte2);
    Wire.write("\n");
    Wire.endTransmission();
}


void setMotor(int joyX, int joyY){

  int x = (joyX - 512)*8;
  int y = (joyY - 512)*8;

  int left_motor = y + x;
  int right_motor = y - x;

  Serial.print(left_motor);
  Serial.print(" ");
  Serial.println(right_motor);

  if (right_motor > 0) {
    tlc_send(0, 0);
    tlc_send(1, right_motor);
    tlc_send(2, 0);
    tlc_send(3, right_motor);
  } else if (right_motor < 0) {
    tlc_send(0, -right_motor);
    tlc_send(1, 0);
    tlc_send(2, -right_motor);
    tlc_send(3, 0);
  } else {
    tlc_send(0, 0);
    tlc_send(1, 0);
    tlc_send(2, 0);
    tlc_send(3, 0);
  }

  if (left_motor > 0) {
    tlc_send(4, 0);
    tlc_send(5, left_motor);
    tlc_send(6, 0);
    tlc_send(7, left_motor);
  } else if (left_motor < 0) {
    tlc_send(4, -left_motor);
    tlc_send(5, 0);
    tlc_send(6, -left_motor);
    tlc_send(7, 0);
  } else {
    tlc_send(4, 0);
    tlc_send(5, 0);
    tlc_send(6, 0);
    tlc_send(7, 0);
  }

  /*
  // Y-axis used for forward and backward control
  if (joyY < 470) {
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    rightPower = map(joyY, 470, 0, 0, 4095);
    leftPower = map(joyY, 470, 0, 0, 4095);
    // Set Right Motors backward
    tlc_send(0,0);
    tlc_send(1,rightPower);
    tlc_send(2,0);
    tlc_send(3,rightPower);
    // Set Left Motors backward
    tlc_send(4,0);
    tlc_send(5,leftPower);
    tlc_send(6,0);
    tlc_send(7,leftPower);
  }
  else if (joyY > 550) {
    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    rightPower = map(joyY, 550, 1023, 0, 255);
    leftPower = map(joyY, 550, 1023, 0, 255);
    // Set Right Motors forward
    tlc_send(0,rightPower);
    tlc_send(1,0);
    tlc_send(2,rightPower);
    tlc_send(3,0);
    // Set Left Motors forward
    tlc_send(0,leftPower);
    tlc_send(1,0);
    tlc_send(2,leftPower);
    tlc_send(3,0);
  }
  // If joy stays in middle the motors are not moving
  else {
    rightPower = 0;
    leftPower = 0;
  }

  // X-axis used for left and right control
  if (joyX < 470) {
    // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 255 value
    int xMapped = map(joyX, 470, 0, 0, 255);
    // Move to left - decrease left motor speed, increase right motor speed
    rightPower = rightPower - xMapped;
    leftPower = leftPower + xMapped;
    // Confine the range from 0 to 255
    if (rightPower < 0) {
      rightPower = 0;
    }
    if (leftPower > 255) {
      leftPower = 255;
    }
  }
  if (joyX > 550) {
    // Convert the increasing X-axis readings from 550 to 1023 into 0 to 255 value
    int xMapped = map(joyX, 550, 1023, 0, 255);
    // Move right - decrease right motor speed, increase left motor speed
    rightPower = rightPower + xMapped;
    leftPower = leftPower - xMapped;
    // Confine the range from 0 to 255
    if (rightPower > 255) {
      rightPower = 255;
    }
    if (leftPower < 0) {
      leftPower = 0;
    }
  }
  */
}

/*
void setServo(int position) {

}
*/

// All button functions will be called each time the controlller recieves data from the remote
bool joyButton(bool value){
  if (value == true){
    return true;
  }
  else{
    return false;
  }
}

bool blueButton(bool value) {
  if (value == true){
    return true;
  }
  else{
    return false;
  }
}

bool yellowButton(bool value) {
  if (value == true){
    return true;
  }
  else{
    return false;
  }
}

bool greenButton(bool value) {
  if (value == true){
    return true;
  }
  else{
    return false;
  }
}

bool redButton(bool value) {
  if (value == true){
    return true;
  }
  else{
    return false;
  }
}

// EEPROM cutom object
struct eepromData {
  byte readChannel[6];
  byte writeChannel[6];
  char setupSSID[20];
  char setupPassword[20];
  char compSSID[20];
  char compPassword[20];
  char name[20];
};

eepromData robotData;

void setup() {
  
  Serial.begin(9600);

  //Check if EEPROM is programmed with valid data
  int checkValue = 0;
  EEPROM.get(0, checkValue);
  if(checkValue != 1){
    Serial.println("Programming EEPROM with default values");
    eepromData defaultData = {
      "00001",
      "00002",
      "Battlebots_Setup",
      "OMGR0b0t$",
      "RRRRRRRRRRRRRRRRRRR",
      "0000000000000000000",
      "Untitled Document"
    };
    EEPROM.put(0, 1);
    EEPROM.put(16, defaultData);
  }
  EEPROM.get(16, robotData);
  
  // Print out juicy details
  Serial.print("Read Channel ");
  Serial.println(robotData.readChannel);
  Serial.print("Write Channel ");
  Serial.println(robotData.writeChannel);
  Serial.print("Robot Name ");
  Serial.println(robotData.name);




  // Radio Setup
  radio.begin();
  radio.openReadingPipe(1, robotData.readChannel);
  radio.setPALevel(RF24_PA_MIN);
  Serial.println("Node statring");
  radio.startListening();
  Serial.println("Hello");

  // Start I2C connection
  Wire.begin();
}

unsigned long last_motor;

int joyX = 512;
int joyY = 512;

void loop() {
  unsigned long now = millis();
  
  if(radio.available()) {
    radio.read(&recievePacket, sizeof(recievePacket));
    joyX = (recievePacket[3]-'0')+10*(recievePacket[2]-'0')+100*(recievePacket[1]-'0')+1000*(recievePacket[0]-'0');
    joyY = (recievePacket[9]-'0')+10*(recievePacket[8]-'0')+100*(recievePacket[7]-'0')+1000*(recievePacket[6]-'0');
  }

  if (now - last_motor > 10) {
      setMotor(joyX, joyY);
      last_motor = now;
  }
  
  /*
  tlc_send(0, 0);
  tlc_send(1, 3000);
  tlc_send(6, 0);
  tlc_send(7, 3000);
  */
  
}
// Battlebots Controller Unit Code 
// by: John Brereton & Logan Greif & Ismail Mansuri
// Made for NodeMCU LoLin Board

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>

RF24 radio(D7, D8); // CE, CSN
byte addresses[6] = {00001,00002};

char sendPacket;
String recievePacket;

String controllerName;

byte writingChannel = addresses[0];
byte readingChannel = addresses[1];

int rightPower;
int leftPower;

void setup() {
  // Get values from EEPROM
  //int eeAddress = 0;
  //EEPROM.get(eeAddress, addresses);


  // Radio Setup
  radio.begin();
  radio.openWritingPipe(writingChannel);
  radio.openReadingPipe(1, readingChannel);
  radio.setPALevel(RF24_PA_MIN);

}

void loop() {
  delay(5);
  radio.stopListening();
  radio.write(&"?v", sizeof("?v"));
  radio.startListening();
  radio.read(&recievePacket, sizeof(recievePacket));
  setMotor(recievePacket);
  // Program Buttons Here
  
}

void setMotor(recievePacket) {
  joystickX = recievePacket[0];
  joystickY = recievePacket[1];

  // Y-axis used for forward and backward control
  if (joystickY < 470) {
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    rightPower = map(joystickY, 470, 0, 0, 4095);
    leftPower = map(joystickY, 470, 0, 0, 4095);
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
  else if (joystickY > 550) {
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
    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    rightPower = map(joystickY, 550, 1023, 0, 255);
    leftPower = map(joystickY, 550, 1023, 0, 255);
  }
  // If joystick stays in middle the motors are not moving
  else {
    rightPower = 0;
    leftPower = 0;
  }

  // X-axis used for left and right control
  if (joystickX < 470) {
    // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 255 value
    int xMapped = map(joystickX, 470, 0, 0, 255);
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
  if (joystickX > 550) {
    // Convert the increasing X-axis readings from 550 to 1023 into 0 to 255 value
    int xMapped = map(joystickX, 550, 1023, 0, 255);
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
}

void setServo(position) {

}

// All button functions will be called each time the controlller recieves data from the remote
void joystickButton(recievePacket){
  if (recievePacket[2]==1){
    return true
  }
  else{
    return false
  }
}

void blueButton(recievePacket) {
  if recievePacket[3]==1){
    return true
  }
  else{
    return false
  }
}

void yellowButton(recievePacket) {
  if recievePacket[4]==1){
    return true
  }
  else{
    return false
  }
}

void greenButton(recievePacket) {
  if recievePacket[5]==1){
    return true
  }
  else{
    return false
  }
}

void redButton(recievePacket) {
  if recievePacket[6]==1){
    return true
  }
  else{
    return false
  }
}

void tlc_send(uint8_t channel, uint16_t value){
    uint8_t byte1 = channel + ((value & 0x000F) << 4);
    uint8_t byte2 = (value & 0x0FF0) >> 4;

    Wire.beginTransmission(ATMEGA_I2C_ADDR);
    Wire.write(byte1);
    Wire.write(byte2);
    Wire.write("\n");
    Wire.endTransmission();
}
-// Battlebots Controller Unit Code 
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
  radio.write(&"?v", sizeof("?v");
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
    // Set Motor A backward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(joystickY, 470, 0, 0, 255);
    motorSpeedB = map(joystickY, 470, 0, 0, 255);
  }
  else if (joystickY > 550) {
    // Set Motor A forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(joystickY, 550, 1023, 0, 255);
    motorSpeedB = map(joystickY, 550, 1023, 0, 255);
  }
  // If joystick stays in middle the motors are not moving
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }

  // X-axis used for left and right control
  if (joystickX < 470) {
    // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 255 value
    int xMapped = map(joystickX, 470, 0, 0, 255);
    // Move to left - decrease left motor speed, increase right motor speed
    motorSpeedA = motorSpeedA - xMapped;
    motorSpeedB = motorSpeedB + xMapped;
    // Confine the range from 0 to 255
    if (motorSpeedA < 0) {
      motorSpeedA = 0;
    }
    if (motorSpeedB > 255) {
      motorSpeedB = 255;
    }
  }
  if (joystickX > 550) {
    // Convert the increasing X-axis readings from 550 to 1023 into 0 to 255 value
    int xMapped = map(joystickX, 550, 1023, 0, 255);
    // Move right - decrease right motor speed, increase left motor speed
    motorSpeedA = motorSpeedA + xMapped;
    motorSpeedB = motorSpeedB - xMapped;
    // Confine the range from 0 to 255
    if (motorSpeedA > 255) {
      motorSpeedA = 255;
    }
    if (motorSpeedB < 0) {
      motorSpeedB = 0;
    }
  }
}

void setServo(direction, angle) {
  if (direction == )
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
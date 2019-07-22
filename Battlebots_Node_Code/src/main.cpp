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
}

void setMotor() {

}

void setServo(int servoNumber, int angle) {


}

// All button functions will be called each time the controlller recieves data from the remote
void redButton(bool redButtonValue) {


}

void blueButton(bool blueButtonValue) {


}

void greenButton(bool greenButtonValue) {


}

void yellowButton(bool yellowButtonValue) {


}
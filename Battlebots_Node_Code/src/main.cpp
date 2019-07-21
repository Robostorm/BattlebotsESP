// Battlebots Controller Unit Code 
// by: John Brereton & Logan Greif
// Made for NodeMCU LoLin Board

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>

RF24 radio(D7, D8); // CE, CSN

const byte addresses[][6] = {"00001", "00002"};

char controlPacket;
String controlRequest;

String controllerName;

int writingChannel;
int readingChannel;

void setup() {
  // Get values from EEPROM
  int eeAddress = 0;
  EEPROM.get(eeAddress, addresses);

  
  // Radio Setup
  radio.begin();
  radio.openWritingPipe(addresses[writingChannel]); // 00002
  radio.openReadingPipe(1, addresses[readingChannel]); // 00001
  radio.setPALevel(RF24_PA_MIN);

}

void loop() {
  delay(5);
  radio.startListening();
  while (!radio.available());
  radio.read(&controlRequest, sizeof(controlRequest));
  if (controlRequest == "?v"){
    char controlPacket[] = {analogRead(JoystickX), analogRead(JoystickY), digitalRead(JoystickBtn), digitalRead(BlueBtn), digitalRead(YellowBtn), digitalRead(GreenBtn), digitalRead(RedBtn)};
  }else if (controlRequest.substring(0,2) == "!c"){
    writingChannel = controlRequest.substring(2,3).toInt();
    readingChannel = controlRequest.substring(3,4).toInt();
  }else if (controlRequest == "?c"){
    char controlPacket[] = {writingChannel,readingChannel};
  }else if (controlRequest.substring(0,2) == "!n"){
    controllerName = controlRequest.substring(2);
  }else if (controlRequest == "?n"){
    char controlPacket = controllerName.c_str();
  }else{
    return;
  }
  
  delay(5);
  radio.stopListening();
  radio.write(&controlPacket, sizeof(controlPacket));
}
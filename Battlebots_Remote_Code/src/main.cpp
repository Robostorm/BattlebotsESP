// Battlebots Remote Code 
// by: John Brereton & Logan Greif
// Made for RF-Nano board

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <EEPROM.h>

RF24 radio(10, 9);

byte addresses[6] = {00001,00002};

const int YellowBtn = 2;
const int RedBtn = 3;
const int GreenBtn = 4;
const int BlueBtn = 5;
const int JoystickBtn = 10;
const int JoystickX = A7;
const int JoystickY = A6;

int sendPacket;
String recievePacket;
String remoteName;

byte writingChannel = addresses[1];
byte readingChannel = addresses[0];

void setup() {
  radio.begin();
  radio.openWritingPipe(addresses[writingChannel]); // 00002
  radio.openReadingPipe(1, addresses[readingChannel]); // 00001
  radio.setPALevel(RF24_PA_MIN);
  pinMode(JoystickY, INPUT);
  pinMode(JoystickX, INPUT);
  pinMode(JoystickBtn, INPUT);
  pinMode(BlueBtn, INPUT);
  pinMode(YellowBtn, INPUT);
  pinMode(GreenBtn, INPUT);
  pinMode(RedBtn, INPUT);
  Serial.begin(9600);
}

void loop() {
  delay(5);
  radio.startListening();
  if ( radio.available()) {
    while (radio.available()) {
      radio.read(&recievePacket, sizeof(recievePacket));
      if (recievePacket == "?v"){
        int joystickXValue = analogRead(JoystickX);
        int joystickYValue = analogRead(JoystickY);
        int sendPacket[] = {joystickXValue, joystickYValue, digitalRead(JoystickBtn), digitalRead(BlueBtn), digitalRead(YellowBtn), digitalRead(GreenBtn), digitalRead(RedBtn)};
      }else if (recievePacket.substring(0,2) == "!c"){
        writingChannel = recievePacket.substring(2,3).toInt();
        readingChannel = recievePacket.substring(3,4).toInt();
      }else if (recievePacket == "?c"){
        char sendPacket[] = {writingChannel,readingChannel};
      }else if (recievePacket.substring(0,2) == "!n"){
        remoteName = recievePacket.substring(2);
      }else if (recievePacket == "?n"){
        //char sendPacket = remoteName.c_str();
      }else{
        return;
      }
  
  delay(5);
  radio.stopListening();
  radio.write(&sendPacket, sizeof(sendPacket));
    }
  }
}
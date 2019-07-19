// Battlebots Remote Code 
// by: John Brereton
// Made for RF-Nano board

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(10, 9);

const byte addresses[][6] = {"00001", "00002"};

const int YellowBtn = 2;
const int RedBtn = 3;
const int GreenBtn = 4;
const int BlueBtn = 5;
const int JoystickBtn = 6;
const int JoystickX = A7;
const int JoystickY = A6;

char controlPacket;
String controlRequest;

String controllerName;

int writingChannel;
int readingChannel;

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

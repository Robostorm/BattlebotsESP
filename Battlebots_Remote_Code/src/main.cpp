// Battlebots Remote Code 
// by: John Brereton & Logan Greif
// Made for RF-Nano board

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>

RF24 radio(10, 9);

//const byte addresses[][6] = {"00001", "00002"};

const int YellowBtn = 7;
const int RedBtn = 5;
const int GreenBtn = 4;
const int BlueBtn = 2;
const int JoystickBtn = 8;
const int JoystickX = A6;
const int JoystickY = A7;

char sendPacket[32] = "";
char recievePacket[32] = "";

// EEPROM cutom object
struct eepromData {
  byte readChannel[6];
  byte writeChannel[6];
  char name[20];
};

eepromData controllerData;


void setup() {
  Serial.begin(9600);
  //Check if EEPROM is programmed with valid data
  int checkValue = 0;
  EEPROM.get(0, checkValue);
  if(checkValue != 1){
    Serial.println("Programming EEPROM with default values");
    eepromData defaultData = {
      "00002",
      "00001",
      "Untitled Document"
    };
    EEPROM.put(0, 1);
    EEPROM.put(16, defaultData);
  }
  EEPROM.get(16, controllerData);

  /* Print out juicy details
  Serial.print("Read Channel ");
  Serial.println(controllerData.readChannel);
  Serial.print("Write Channel ");
  Serial.println(controllerData.writeChannel);
  Serial.print("Robot Name ");
  Serial.println(controllerData.name);
*/
  radio.begin();
  radio.openWritingPipe(controllerData.writeChannel);
  radio.setPALevel(RF24_PA_MIN);
  pinMode(JoystickBtn, INPUT_PULLUP);
  pinMode(BlueBtn, INPUT_PULLUP);
  pinMode(YellowBtn, INPUT_PULLUP);
  pinMode(GreenBtn, INPUT_PULLUP);
  pinMode(RedBtn, INPUT_PULLUP);
  
  Serial.println("Remote starting");
  radio.stopListening();
  Serial.println("Radio is living");
}

void loop() {
  delay(5);
  int joystickXValue = analogRead(JoystickX);
  int joystickYValue = analogRead(JoystickY);
  sprintf(
    sendPacket,
    "%04d, %04d, %01d, %01d, %01d, %01d, %01d",
    joystickXValue,
    joystickYValue,
    digitalRead(JoystickBtn),
    digitalRead(BlueBtn),
    digitalRead(YellowBtn),
    digitalRead(GreenBtn),
    digitalRead(RedBtn)
  );
  Serial.println(sendPacket);
  radio.stopListening();
  radio.write(&sendPacket, sizeof(sendPacket));
}
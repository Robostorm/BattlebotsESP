// Battlebots Remote Code 
// by: John Brereton & Logan Greif
// Made for RF-Nano board

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <EEPROM.h>

RF24 radio(10, 9);

const byte addresses[][6] = {"00001", "00002"};

const int YellowBtn = 7;
const int RedBtn = 5;
const int GreenBtn = 4;
const int BlueBtn = 2;
const int JoystickBtn = 8;
const int JoystickX = A6;
const int JoystickY = A7;

char sendPacket[32] = "";
char recievePacket[32] = "";

void setup() {
  radio.begin();
  radio.openWritingPipe(addresses[1]);
  radio.setPALevel(RF24_PA_MIN);
  pinMode(JoystickBtn, INPUT_PULLUP);
  pinMode(BlueBtn, INPUT_PULLUP);
  pinMode(YellowBtn, INPUT_PULLUP);
  pinMode(GreenBtn, INPUT_PULLUP);
  pinMode(RedBtn, INPUT_PULLUP);
  Serial.begin(9600);
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
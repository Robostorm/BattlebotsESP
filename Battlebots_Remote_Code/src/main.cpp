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

const int YellowBtn = 9;
const int RedBtn = 3;
const int GreenBtn = 2;
const int BlueBtn = 32;
const int JoystickBtn = 10;
const int JoystickX = 19;
const int JoystickY = 22;

char sendPacket[32] = "";
char recievePacket[32] = "";

void setup() {
  radio.begin();
  //radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(0, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
  pinMode(JoystickY, INPUT);
  pinMode(JoystickX, INPUT);
  //pinMode(JoystickBtn, INPUT);
  pinMode(BlueBtn, INPUT);
  //pinMode(YellowBtn, INPUT);
  pinMode(GreenBtn, INPUT);
  pinMode(RedBtn, INPUT);
  Serial.begin(9600);
  Serial.println("Remote starting");
  radio.startListening();
  Serial.println("Radio listening");
}

void loop() {
  delay(5);
  if ( radio.available()) {
    Serial.println("Bytes available");
    radio.read(&recievePacket, sizeof(recievePacket));
    Serial.println(recievePacket);
    if (recievePacket[0] == '?' && recievePacket[1] == 'v'){
      int joystickXValue = analogRead(JoystickX);
      int joystickYValue = analogRead(JoystickY);
      sprintf(
        sendPacket,
        "%03d, %03d, %01d, %01d, %01d, %01d, %01d",
        joystickXValue,
        joystickYValue,
        //digitalRead(JoystickBtn),
        0,
        digitalRead(BlueBtn),
        //digitalRead(YellowBtn),
        0,
        digitalRead(GreenBtn),
        digitalRead(RedBtn)
      );
      Serial.println(sendPacket);
      //radio.stopListening();
      //radio.write(&sendPacket, sizeof(sendPacket));
    }else if (recievePacket[0] == '?' && recievePacket[1] == 'c'){
      //writingChannel = recievePacket.substring(2,3).toInt();
      //readingChannel = recievePacket.substring(3,4).toInt();
    }else{
      
    }
    
    delay(5);

  } else {
      //Serial.println("No bytes available");
  }
}
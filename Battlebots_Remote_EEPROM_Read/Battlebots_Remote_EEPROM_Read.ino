#include <EEPROM.h>

void setup() {

  byte addresses[][6] = {"00001", "00002"};
  String name;
  String version;
  int eeAddress = 0; //EEPROM address to start reading from

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Read addresses from EEPROM: ");

  //Get the float data from the EEPROM at position 'eeAddress'
  EEPROM.get(eeAddress, addresses);
  //Serial.print(addresses[0]);
  //Serial.print(addresses[1]);
  eeAddress += sizeof(addresses);
  EEPROM.get(eeAddress, name);
  eeAddress += sizeof(name);
  Serial.println(name);
  EEPROM.get(eeAddress, version);
  //eeAddress += sizeof(version);
  Serial.println(version);
  
}


void loop() {
  /* Empty loop */
}

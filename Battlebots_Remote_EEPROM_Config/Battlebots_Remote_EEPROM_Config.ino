#include <EEPROM.h>

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  const byte addresses[][6] = {"00001", "00002"};
  String name = "Logan Greif";
  String version = "0.0";
  int eeAddress = 0;   //Location we want the data to be put.


  //One simple call, with the address first and the object second.
  EEPROM.put(eeAddress, addresses);
  eeAddress += sizeof(addresses);
  EEPROM.put(eeAddress, name);
  eeAddress += sizeof(name);
  EEPROM.put(eeAddress, version);

  Serial.println("Written EEPROM");

}

void loop() {
  /* Empty loop */
}

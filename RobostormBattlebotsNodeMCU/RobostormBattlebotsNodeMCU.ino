#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/************ WIFI and MQTT INFORMATION ******************/
#define wifi_ssid "Battlebots-Setup" //enter your WIFI SSID
#define wifi_password "R0b0t$Rul3!" //enter your WIFI Password

#define mqtt_server "192.168.50.10" // Enter your MQTT server adderss or IP. I use my DuckDNS adddress (yourname.duckdns.org) in this field
#define mqtt_user "setupuser" //enter your MQTT username
#define mqtt_password "setupPassword" //enter your password

// Tell the system that this is an ESP WiFi board and not an ethernet2 board
WiFiClient espClient;
PubSubClient client(espClient);


RF24 radio(D7, D8); // CE, CSN NodeMCU
//RF24 radio(10, 9); // CE, CSN RF-NANO
const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  setup_wifi();
  client.setServer(mqtt_server, 1883); //CHANGE PORT HERE IF NEEDED
  client.setCallback(callback);
}
void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
}

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  int i = 0;

  if (String(topic) == setpowersub) {
    for (i = 0; i < length; i++) {
      message_buff[i] = payload[i];
    }
    message_buff[i] = '\0';
    setPower = String(message_buff);
    Serial.println("Set Power: " + setPower);
    if (setPower == "OFF") {
      client.publish(setpowerpub, "OFF");
    }

    if (setPower == "ON") {
      client.publish(setpowerpub, "ON");
    }
  }


  if (String(topic) == seteffectsub) {
    for (i = 0; i < length; i++) {
      message_buff[i] = payload[i];
    }
    message_buff[i] = '\0';
    setEffect = String(message_buff);
    Serial.println("Set Effect: " + setEffect);
    setPower = "ON";
    client.publish(setpowerpub, "ON");
    if (setEffect == "Twinkle") {
      twinklecounter = 0;
    }
    if (setEffect == "Lightning") {
      twinklecounter = 0;
    }
  }


  if (String(topic) == setbrightness) {
    for (i = 0; i < length; i++) {
      message_buff[i] = payload[i];
    }
    message_buff[i] = '\0';
    setBrightness = String(message_buff);
    Serial.println("Set Brightness: " + setBrightness);
    brightness = setBrightness.toInt();
    setPower = "ON";
    client.publish(setpowerpub, "ON");
  }

  if (String(topic) == setcolorsub) {
    for (i = 0; i < length; i++) {
      message_buff[i] = payload[i];
    }
    message_buff[i] = '\0';
    client.publish(setcolorpub, message_buff);
    setColor = String(message_buff);
    Serial.println("Set Color: " + setColor);
    setPower = "ON";
    client.publish(setpowerpub, "ON");
    }

  if (String(topic) == setanimationspeed) {
    for (i = 0; i < length; i++) {
      message_buff[i] = payload[i];
    }
    message_buff[i] = '\0';
    setAnimationSpeed = String(message_buff);
    animationspeed = setAnimationSpeed.toInt();
  }
}

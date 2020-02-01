#include <PubSubClient.h>
#include <M5StickC.h>
#include "RoverC.h"
#include <WiFi.h>

const char *ssid = "C2_MNCPlay";
const char *password = "20B02F81E";

const char* mqtt_server = "hairdresser.cloudmqtt.com";
#define mqtt_port 17548
#define MQTT_USER "hzancnpt"
#define MQTT_PASSWORD "K0XM1GiszHPF"
#define MQTT_SERIAL_PUBLISH_CH "/radipp/ESP32/serialdata/tx"
#define MQTT_SERIAL_RECEIVER_CH "/radipp/ESP32/serialdata/rx"

int command;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) {
      Serial.println("connected");
      //Once connected, publish an announcement...
      client.publish("/icircuit/presence/ESP32/", "hello world");
      // ... and resubscribe
      client.subscribe(MQTT_SERIAL_RECEIVER_CH);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/*void callback(char* topic, byte *payload, unsigned int length) {
    Serial.println("-------new message from broker-----");
    Serial.print("channel:");
    Serial.println(topic);
    Serial.print("data:"); 
    Serial.write(payload, length);
    Serial.println();
}*/

void callback(char* topic, byte *payload, unsigned int length) {
    Serial.println("-------new message from broker-----");
    Serial.print("channel:");
    Serial.println(topic);
    Serial.print("data:"); 
    Serial.write(payload, length);
    Serial.println();

    if(length==1){
      Move_forward(80);
      delay (500);
      Move_stop(10);
  }

    if(length==2){
      Move_back(80);
      delay (500);
      Move_stop(10);
  }

    if(length==3){
      Move_left(80);
      delay (500);
      Move_stop(10);
  }

    if(length==4){
      Move_right(80);
      delay (500);
      Move_stop(10);
  }

  
}


void setup(){ 
  
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextColor(WHITE);
  //M5.Axp.ScreenBreath(0);  
  RoverC_Init();

  Serial.begin(115200);
  Serial.setTimeout(500);// Set time out for 
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();

}

void publishSerialData(char *serialData){
  if (!client.connected()) {
    reconnect();
  }
  client.publish(MQTT_SERIAL_PUBLISH_CH, serialData);
}


void loop() {

  client.loop();
   if (Serial.available() > 0) {
     char mun[501];
     memset(mun,0, 501);
     Serial.readBytesUntil( '\n',mun,500);
     publishSerialData(mun);
   

   }
}


  





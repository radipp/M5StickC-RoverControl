/*
 *  This sketch sends random data over UDP on a ESP32 device
 *
 */
#include <WiFi.h>
#include <WiFiUdp.h>
#include <M5Stack.h>

// WiFi network name and password:

const char * networkName = "wifikantor";
const char * networkPswd = "wifipassword";

//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
const char * udpAddress = "192.168.43.47";
const int udpPort = 1003;

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

void setup(){
  M5.Power.begin();
  // Initilize hardware serial:
  Serial.begin(115200);
  
  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
}

void loop(){
  //only send data when connected
  if(connected){
    //Send a packet

    if(M5.BtnB.read()){
      udp.beginPacket(udpAddress,udpPort);
      udp.printf("Forward");
      udp.endPacket();
      //M5.Lcd.println("Forward");
  }

    if(M5.BtnA.read()){
      udp.beginPacket(udpAddress,udpPort);
      udp.printf("Left");
      udp.endPacket();
      //M5.Lcd.println("Left");
  }

    if(M5.BtnC.read()){
      udp.beginPacket(udpAddress,udpPort);
      udp.printf("Right");
      udp.endPacket();
      //M5.Lcd.println("Right");
  }

  //Wait for 1 second
  delay(1);
}
}

void connectToWiFi(const char * ssid, const char * pwd){
  M5.Lcd.println("Connecting to WiFi network: "); 
  M5.Lcd.println(ssid);

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  M5.Lcd.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          M5.Lcd.println("WiFi connected! IP address: ");
          //M5.Lcd.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(),udpPort);
          connected = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          M5.Lcd.println("WiFi lost connection");
          connected = false;
          break;
    }
}


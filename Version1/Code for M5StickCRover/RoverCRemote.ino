#include <M5StickC.h>
#include "RoverC.h"
#include <WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "wifikantor";
const char *password = "wifipassword";

int command;


WiFiServer server(80);
WiFiUDP Udp1;

// the setup routine runs once when M5StickC starts up
void setup(){ 
  
  // Initialize the M5StickC object
  M5.begin();
  
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextColor(WHITE);
  
  connectToWiFi(ssid, password);
  Udp1.begin(1003);

  M5.Lcd.println("Server Started");
  RoverC_Init();
}

// the loop routine runs over and over again forever
void loop() {

int udplength = Udp1.parsePacket();
  if( udplength )
  {
    char udodata[udplength];
    //Udp1.read( udodata, udplength);
    command = Udp1.read( udodata, udplength);

    if(command==7){
      Move_forward(50);
  }

    if(command==4){
      Move_turnleft(20);
  }

    if(command==5){
      Move_turnright(30);
    }
  }
  else {
    Move_stop(10);
    };
  }
  

void connectToWiFi(const char * ssid, const char * pwd){
  M5.Lcd.println("Connecting to WiFi network: "); 
  M5.Lcd.println(ssid);

  // delete old config
  WiFi.disconnect(true);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  M5.Lcd.println("Waiting for WIFI connection...");
}



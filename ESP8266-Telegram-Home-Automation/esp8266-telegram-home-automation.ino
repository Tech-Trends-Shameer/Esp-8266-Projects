//Tech Trends Shameer
//Home Automation using Telegram Bot

#include "CTBot.h"
#include "DHT.h"

int RELAYPIN =D4;    // Digital pin connected to the DHT sensor
 
CTBot myBot;
TBMessage msg;
String ssid  = "WIFI Username"; //Enter Wifi Username
String pass  = "Wifi Password"; // Enter Wifi Password
String token = "Token"   ; // Enter Your Telegram Bot Token

void setup() {
  // initialize the Serial
  Serial.begin(115200);
//  dht.begin();
pinMode(RELAYPIN, OUTPUT);
  Serial.println("Starting TelegramBot...");
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);
  if (myBot.testConnection()){
   
    Serial.println("\ntestConnection OK");
  }
  else
  {
    Serial.println("\ntestConnection NOK");
  }

 
}

void loop() {
   
  if (myBot.getNewMessage(msg)) {

    if (msg.text.equalsIgnoreCase("light on")) {    
      digitalWrite(RELAYPIN, HIGH);
     
      Serial.println("ON");
    }
 if (msg.text.equalsIgnoreCase("light off")) {    
  digitalWrite(RELAYPIN, LOW);
  
  Serial.println("OFF");
    }

}
  
delay(10);

}

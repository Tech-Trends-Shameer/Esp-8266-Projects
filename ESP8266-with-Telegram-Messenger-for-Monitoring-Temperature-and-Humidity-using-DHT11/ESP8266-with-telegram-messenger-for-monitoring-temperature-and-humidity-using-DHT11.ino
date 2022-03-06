//Tech Trends Shameer
//ESP8266 with telegram messenger for monitoring temperature and humidity using DHT11

#include "CTBot.h"
#include "DHT.h"

#define DHTPIN D4    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
CTBot myBot;
TBMessage msg;
String ssid  = "WIFI Username"    ; // Enter WIFI Username
String pass  = "WIFI Passoword"; // Enter WIFI Password
String token = "Token"   ; // Enter the token from Telegram Bor

void setup() {
  // initialize the Serial
  Serial.begin(115200);
  dht.begin();
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
  // a variable to store telegram message data
    
   float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  
  String humid = "Humidity: " + String(h) + " % \n";
  String temp = "Temperature: " + String(t) + "°C, " + String(f) + "°F \n";
  if (myBot.getNewMessage(msg)) {

    if (msg.text.equalsIgnoreCase("Humidity")) {    
     myBot.sendMessage(msg.sender.id, humid);
    }
 if (msg.text.equalsIgnoreCase("Temperature")) {    
      myBot.sendMessage(msg.sender.id, temp); 
    }

}
  
delay(10);

}

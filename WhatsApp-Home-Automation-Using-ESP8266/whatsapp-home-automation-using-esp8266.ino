//#include <WiFi.h>
#include <ESP8266WiFi.h>
#include <ThingESP.h>

ThingESP8266 thing("techtrends", "ControlLED", "techtrends");

int LIGHT = D2;

unsigned long previousMillis = 0;
const long INTERVAL = 6000;  

void setup()
{
  Serial.begin(115200);
  pinMode(LIGHT, OUTPUT);
  thing.SetWiFi("WiFi Username", "WiFi Password");
  thing.initDevice();
}


String HandleResponse(String query)
{
  if (query == "light on") {
    digitalWrite(LIGHT, 1);
    return "Done: Light Turned ON";
  }

  else if (query == "light off") {
    digitalWrite(LIGHT, 0);
    return "Done: Light Turned OFF";
  }

  else if (query == "light status")
    return digitalRead(LIGHT) ? "LIGHT is OFF" : "LIGHT is ON";
  else return "Your query was invalid..";

}


void loop()
{
  thing.Handle();

}

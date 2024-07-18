#include <ESP8266WiFi.h>
#include <ThingESP.h>

ThingESP8266 thing("techtrends", "HomeAutomation", "techtrends");

int LIGHT1 = D1;
int LIGHT2 = D2;
int LIGHT3 = D3;
int LIGHT4 = D4;

unsigned long previousMillis = 0;
const long INTERVAL = 6000;  

void setup()
{
  Serial.begin(115200);
  pinMode(LIGHT1, OUTPUT);
  pinMode(LIGHT2, OUTPUT);
  pinMode(LIGHT3, OUTPUT);
  pinMode(LIGHT4, OUTPUT);
  thing.SetWiFi("WiFi Username", "WiFi Password");
  thing.initDevice();
}


String HandleResponse(String query)
{
  if (query == "light1 on") {
    digitalWrite(LIGHT1, 1);
    return "Done: Light1 Turned ON";
  }

  else if (query == "light1 off") {
    digitalWrite(LIGHT1, 0);
    return "Done: Light1 Turned OFF";
  }

  if (query == "light2 on") {
    digitalWrite(LIGHT2, 1);
    return "Done: Light2 Turned ON";
  }

  else if (query == "light2 off") {
    digitalWrite(LIGHT2, 0);
    return "Done: Light2 Turned OFF";
  }

    if (query == "light3 on") {
    digitalWrite(LIGHT3, 1);
    return "Done: Light3 Turned ON";
  }

  else if (query == "light3 off") {
    digitalWrite(LIGHT3, 0);
    return "Done: Light3 Turned OFF";
  }

    if (query == "light4 on") {
    digitalWrite(LIGHT4, 1);
    return "Done: Light4 Turned ON";
  }

  else if (query == "light4 off") {
    digitalWrite(LIGHT4, 0);
    return "Done: Light4 Turned OFF";
  }

  //else if (query == "light status")
   // return digitalRead(LIGHT) ? "LIGHT is OFF" : "LIGHT is ON";
  else return "Your query was invalid..";

}


void loop()
{
  thing.Handle();

}

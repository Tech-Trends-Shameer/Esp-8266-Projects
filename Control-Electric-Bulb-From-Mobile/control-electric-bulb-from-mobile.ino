#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "Auth Code";
char ssid[] = "Wifi Username";
char pass[] = "Wifi Password";
 
void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth,ssid,pass);
   
}

void loop()
{
  Blynk.run();
}

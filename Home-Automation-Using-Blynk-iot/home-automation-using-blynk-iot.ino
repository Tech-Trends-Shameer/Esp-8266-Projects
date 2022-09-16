#define BLYNK_TEMPLATE_ID "TMPLZdgdgOwummO6"
#define BLYNK_DEVICE_NAME "Control Electric Bulb"
#define BLYNK_AUTH_TOKEN "5_S4mjFLbdfdggg7jZhdLDjgZxpKTWh_23liU4"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
 
#include <BlynkSimpleEsp8266.h>
 

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "";  // type your wifi name
char pass[] = "";  // type your wifi password

int relaypin = D4;
void setup()
{     
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);    
  pinMode(relaypin,OUTPUT);
 
  }

void loop()
{
  Blynk.run(); 
 }

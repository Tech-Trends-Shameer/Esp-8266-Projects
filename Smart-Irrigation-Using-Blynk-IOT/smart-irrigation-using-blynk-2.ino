//Tech Trends Shameer
//Control Water Pump Using Blynk 2.0

#define BLYNK_TEMPLATE_ID "TemplateID"
#define BLYNK_DEVICE_NAME "Control Pump"
#define BLYNK_AUTH_TOKEN "Auth Token"


#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
 
#include <BlynkSimpleEsp8266.h>
 

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Wifi Username";  // Enter your wifi name
char pass[] = "Wifi Password";  // Enter your wifi password

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

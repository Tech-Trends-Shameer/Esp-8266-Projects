//Tech Trends Shameer
//Smart LED Tube Light Using ESP8266 and Blynk IOT | Home Automation Using ESP8266 and Blynk IOT

#define BLYNK_TEMPLATE_ID "TMPL3Sm7jhd4p"
#define BLYNK_TEMPLATE_NAME "Smart Tube Light Using ESP8266"
#define BLYNK_AUTH_TOKEN "EA4JsAwoncg-MZsqdg2MDfsf_sa0yhd-wnhsf9Q"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
 
#include <BlynkSimpleEsp8266.h>
 

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "WiFi Username";  // type your wifi name
char pass[] = "WiFi Password";  // type your wifi password

int relaypin = D2; 
void setup()
{     
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);    
  pinMode(relaypin,OUTPUT);
 
  }

void loop()
{
  Blynk.run(); 
 }

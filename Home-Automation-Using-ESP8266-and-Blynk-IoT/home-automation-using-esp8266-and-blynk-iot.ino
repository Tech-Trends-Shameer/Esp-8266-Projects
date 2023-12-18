#define BLYNK_TEMPLATE_ID "TMPL3UPPImTJH"
#define BLYNK_TEMPLATE_NAME "Home Automation Using ESP8266"
#define BLYNK_AUTH_TOKEN "VngJ1GCIF0KAR-XBQr35RIRoMSddb22e"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
 
#include <BlynkSimpleEsp8266.h>
 

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "JioFi2_D08135";  // type your wifi name
char pass[] = "WiFi Password";  // type your wifi password

int relaypin1 = D1; //Connect to Pin D1 in NodeMCU
int relaypin2 = D2; //Connect to Pin D2 in NodeMCU
int relaypin3 = D3; //Connect to Pin D3 in NodeMCU
int relaypin4 = D4; //Connect to Pin D4 in NodeMCU
void setup()
{     
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);    
  pinMode(relaypin1,OUTPUT);
  pinMode(relaypin2,OUTPUT);
  pinMode(relaypin3,OUTPUT);
  pinMode(relaypin4,OUTPUT);
 
  }

void loop()
{
  Blynk.run(); 
 }

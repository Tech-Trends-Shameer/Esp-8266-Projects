#define BLYNK_TEMPLATE_ID "TMPLh9mutBsh"
#define BLYNK_TEMPLATE_NAME "Control Fan Using ESP8266"
#define BLYNK_AUTH_TOKEN "HddHMUCj2KNi8pusbqOfdhjljllbLZFSBDSRXzu"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
 
#include <BlynkSimpleEsp8266.h>
 

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "WiFi Username";  // type your wifi name
char pass[] = "WiFi Password";  // type your wifi password

int relaypin = D2;  //GPIO4
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

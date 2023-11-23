//Tech Trends Shameer
//Light Intensity Measurement

#define BLYNK_TEMPLATE_ID "TMPL3Atk3AY9w"
#define BLYNK_TEMPLATE_NAME "Light Intensity Measurement"
#define BLYNK_AUTH_TOKEN "Am76Kd2Fr-xfgCq7uydjl9mmuIs4--mxfkj6mZk"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "WiFi Username";  // type your wifi name
char pass[] = "WiFi Password";  // type your wifi password

BlynkTimer timer;


int temt6000Pin = A0;   //Connect Ambient Sensor to A0 in NodeMCU
float light;
int light_value;

void lightIntensity()
{
    int light_value = analogRead(temt6000Pin);
    light = light_value * 0.0976;// percentage calculation
    Blynk.virtualWrite(V0, light);
    Serial.print("Light Value");
    Serial.println(light_value);
    Serial.print("Light %");
    Serial.println(light);
    delay(2000);
}

void setup() 
{
  Serial.begin(115200);
  
  pinMode(temt6000Pin, INPUT); //data pin for ambientlight sensor
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, lightIntensity);
}
 
void loop() 
{
    Blynk.run();
    timer.run();
}

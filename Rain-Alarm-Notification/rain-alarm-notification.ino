#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;
char auth[] = "x7i-rCaK_XkesEG7Ezdd-EwKivRgKZryohsdgfgJ"; 
 
char ssid[] = "wifi username";
char pass[] = "wifi password";
int flag=0;
void notifyOnFire()
{
  int isButtonPressed = digitalRead(D1);
  if (isButtonPressed==0 && flag==0) {
    Serial.println("Its Raining");
    Blynk.notify("Alert : Its Raining");
    flag=1;
  }
  else if (isButtonPressed==1)
  {
    flag=0;
  }
}
void setup()
{
Serial.begin(9600);
Blynk.begin(auth, ssid, pass);
pinMode(D1,INPUT);
timer.setInterval(1000L,notifyOnFire); 
}
void loop()
{
  Blynk.run();
  timer.run();
}

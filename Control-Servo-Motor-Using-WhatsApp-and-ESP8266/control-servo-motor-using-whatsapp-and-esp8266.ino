#include <ESP8266WiFi.h>
#include <ThingESP.h>
 
#include<Servo.h>
 
ThingESP8266 thing("techtrends", "ControlLED", "techtrends");
Servo servo1;
 
int  servo_pin =D2;
unsigned long previousMillis = 0;
const long INTERVAL = 6000;  

void setup()
{
  Serial.begin(9600);
 
   servo1.attach(servo_pin); 
  thing.SetWiFi("WiFi Username", "WiFi Password");

  thing.initDevice();

}


String HandleResponse(String query)
{
  if (query == "servo 180") {
    digitalWrite(servo_pin, 1);
    servo1.write(180);
    return "Done: Servo Motor Turned 180 degree";
  }

  else if (query == "servo 90") {
    digitalWrite(servo_pin, 1);
    servo1.write(90);
    return "Done: Servo Motor Turned 90 degree";
  }
  else if (query == "servo 0") {
    digitalWrite(servo_pin, 1);
    servo1.write(0);
    return "Done: Servo Motor Turned 0 degree";
  }

  else if (query == "servo status")
    return digitalRead(servo_pin) ? "Servo is OFF" : "Servo is ON";
  else return "Your query was invalid..";

}


void loop()
{ 

  thing.Handle();

}

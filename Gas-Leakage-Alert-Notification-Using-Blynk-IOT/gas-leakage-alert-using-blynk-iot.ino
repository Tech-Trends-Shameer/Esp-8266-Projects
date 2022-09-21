#define BLYNK_TEMPLATE_ID "TMPL4raE8f18"
#define BLYNK_DEVICE_NAME "Gas Leakage"
#define BLYNK_AUTH_TOKEN "s-O1oO5G4Xgu7nndsffEPZ6HdD81V74ZQpFy"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

 
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Wifi Username";  // type your wifi name
char pass[] = "Wifi Password";  // type your wifi password
int smokeA0 = A0;
int data = 0;
int sensorThres = 100;


BlynkTimer timer;

void sendSensor(){
 
 int data = analogRead(smokeA0);
 Blynk.virtualWrite(V0, data);
  Serial.print("Pin A0: ");
  Serial.println(data);


  if(data > 20){
    Blynk.email("test@gmail.com", "Alert", "Gas Leakage Detected!");
    Blynk.logEvent("gas_alert","Gas Leakage Detected");
  }
}

void setup(){
  pinMode(smokeA0, INPUT);
   Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  //dht.begin();
  timer.setInterval(2500L, sendSensor);
}

void loop(){
  Blynk.run();
  timer.run();
}

#define BLYNK_TEMPLATE_ID "TMPL3EFXpZo_U"
#define BLYNK_TEMPLATE_NAME "Smart Waste Management"
#define BLYNK_AUTH_TOKEN "Ou4_p5iR3nsNsdp1Ak5MMB5gheXY4qXHPhAeNZ"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define TRIGGER_PIN D2 // Ultrasonic sensor trigger pin
#define ECHO_PIN D1    // Ultrasonic sensor echo pin
#define RAIN_SENSOR_PIN D4 // Rain sensor analog pin
int flag=0;

char auth[] = BLYNK_AUTH_TOKEN; // Blynk authentication token
char ssid[] = "WiFi Username";  // Your WiFi network SSID
char pass[] = "WiFi Password"; // Your WiFi network password

BlynkTimer timer;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  timer.setInterval(5000L, sendSensorData); // Send sensor data every 5 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}

void sendSensorData() {
  /*long duration, distance;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);  
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1;*/
  
  float distance = getDistance(); // Measure distance in centimeters
  int rainValue = digitalRead(RAIN_SENSOR_PIN);
  //bool isRaining = rainValue > 50; // Adjust threshold according to the sensitivity of the rain sensor
  Serial.println("Distance");
  Serial.print(distance);
  Serial.println("Rain");
  Serial.print(rainValue);
  // Send waste level and rain status to Blynk
  Blynk.virtualWrite(V5, distance);
  Blynk.virtualWrite(V6, rainValue);
  
  // Check if waste bin is full
  if (distance < 6) {
    Serial.println("Waste bin is full! Please empty it.");
    //Blynk.notify("Waste bin is full! Please empty it.");
    Blynk.logEvent("waste_alert","Waste bin is full! Please empty it.");
  }
  
  // Check if it's raining
  //if (isRaining) {
   if (rainValue==0){
    Serial.println("Its Raining");
    //Blynk.notify("It's raining! Please take appropriate action.");
    Blynk.logEvent("rain_alert","It's raining! Waste bin is full! Please take appropriate action.");
    //flag=1;
  }
    /*else if (rainValue==1)
  {
    flag=0;
  }*/
}

float getDistance() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  float duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2; // Calculate distance in centimeters
  return distance;
}

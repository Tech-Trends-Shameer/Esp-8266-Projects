#define BLYNK_TEMPLATE_ID "TMPL3GtKj4QgJ"
#define BLYNK_TEMPLATE_NAME "Theft Alert Notification Using ESP8266"
#define BLYNK_AUTH_TOKEN "Ebci2NrlEBxldgolPFyqG0vFGHdsgh299-qdZaC6m"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Replace with your Wi-Fi credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "WiFi Username";
char pass[] = "WiFi Password";

// Define the pin configuration
const int sensorPin = 4;  // D2 on NodeMCU corresponds to GPIO4
const int ledPin = 5;     // D1 on NodeMCU corresponds to GPIO5

void setup() {
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);

  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  Blynk.run();
  
  int sensorValue = digitalRead(sensorPin);

  if (sensorValue == HIGH) {
    digitalWrite(ledPin, HIGH);
    Serial.println("Motion detected!");
    Blynk.logEvent("theft_alert","Theft Alert in Home");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("No motion detected");
  }

  delay(500);
}

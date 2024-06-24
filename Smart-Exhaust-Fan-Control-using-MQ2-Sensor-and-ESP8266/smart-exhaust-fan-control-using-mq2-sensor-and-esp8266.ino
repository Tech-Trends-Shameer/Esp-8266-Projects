#define BLYNK_TEMPLATE_ID "TMPLsdf37UsdfmEUog3K"
#define BLYNK_TEMPLATE_NAME "Smoke Controlled Exhaust Fan"
#define BLYNK_AUTH_TOKEN "iE0jdwBsdfLmUBFdsfZICzOhCPfBGfg0WLsdfhw3snzS"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN; // Blynk Auth Token
char ssid[] = "WiFi Username";      // WiFi SSID
char pass[] = "WiFi Password";  // WiFi Password

const int smokeA0 = A0;   // Analog pin A0 connected to MQ-2 sensor
const int relayPin = D1;  // Digital pin D1 connected to relay module

BlynkTimer timer;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  pinMode(smokeA0, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  timer.setInterval(1000L, checkSmokeLevel);
}

void checkSmokeLevel() {
  int smokeLevel = analogRead(smokeA0);
  Serial.print("Smoke Level: ");
  Serial.println(smokeLevel);
  Blynk.virtualWrite(V1, smokeLevel); // Send smoke level to Blynk

  if (smokeLevel > 150) {
    digitalWrite(relayPin, HIGH); // Turn on relay (and fan)
  } else {
    digitalWrite(relayPin, LOW);  // Turn off relay (and fan)
  }
}

BLYNK_WRITE(V2) { // Manual control from Blynk app
  int pinValue = param.asInt();
  digitalWrite(relayPin, pinValue);
}

void loop() {
  Blynk.run();
  timer.run();
}

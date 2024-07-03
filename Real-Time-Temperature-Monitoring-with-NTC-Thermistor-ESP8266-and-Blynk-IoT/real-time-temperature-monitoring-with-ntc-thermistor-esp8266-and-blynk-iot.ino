#define BLYNK_TEMPLATE_ID "TMPL3XGFnbsdgsEgK"
#define BLYNK_TEMPLATE_NAME "Temperature Monitor Using Thermistor"
#define BLYNK_AUTH_TOKEN "nSrruVdL_4UUg8cesdkpL1KgsliXgd8P_714Fh"

//#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;  // Your Blynk auth token
char ssid[] = "WiFi Username";  // Your WiFi SSID
char pass[] = "WiFi Password";  // Your WiFi Password

const int analogPin = A0;  // Analog pin for NTC thermistor
BlynkTimer timer;

void sendTemperature() {
  int analogValue = analogRead(analogPin);
  float voltage = analogValue * (3.3 / 1023.0);  // Convert analog reading to voltage
  float resistance = (3.3 - voltage) * 10000 / voltage;  // Calculate resistance
  float temperature = 1 / (log(resistance / 10000) / 3950 + 1 / (25 + 273.15)) - 273.15;  // Calculate temperature
  
  Blynk.virtualWrite(V1, temperature);  // Send temperature to Blynk app (Virtual Pin V5)

    Serial.print("Analog Value: ");
  Serial.print(analogValue);
  Serial.print(" Voltage: ");
  Serial.print(voltage);
  Serial.print(" V, Resistance: ");
  Serial.print(resistance);
  Serial.print(" ohms, Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  
  timer.setInterval(1000L, sendTemperature);  // Send temperature every second
}

void loop() {
  Blynk.run();
  timer.run();
}

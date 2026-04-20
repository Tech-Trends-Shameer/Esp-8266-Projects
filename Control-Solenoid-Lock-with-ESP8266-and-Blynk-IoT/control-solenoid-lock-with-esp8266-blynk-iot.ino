#define BLYNK_TEMPLATE_ID "TMPBGL3A8WzD"
#define BLYNK_TEMPLATE_NAME "Control Solenoid Door Lock"
#define BLYNK_AUTH_TOKEN "I7VJP6KsWjbKZ_RI72YjfnAdfpTRdQgBHVApB-sc"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "WiFi Username";    
char pass[] = "WiFi Password"; 

#define SOLENOID_PIN D1  // GPIO5 - change to D2, D5, etc. as needed

BLYNK_WRITE(V1) {  
  int value = param.asInt();
  digitalWrite(SOLENOID_PIN, value); // Unlock (HIGH) / Lock (LOW)
}

void setup() {
  Serial.begin(115200);
  pinMode(SOLENOID_PIN, OUTPUT);
  digitalWrite(SOLENOID_PIN, LOW);  // Start locked

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");

  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();  
}

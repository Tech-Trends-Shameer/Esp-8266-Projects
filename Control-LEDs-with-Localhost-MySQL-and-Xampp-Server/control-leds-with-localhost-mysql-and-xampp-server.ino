#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// #include <WiFi.h>
// #include <HTTPClient.h>

const char* ssid = "WiFi Username";
const char* password = "WiFi Password";

const char* serverName = "http://localIP/control_led/getLedStatus.php";

const int led1Pin = D1; 
const int led2Pin = D2; 
const int led3Pin = D3; 
const int led4Pin = D4; 

WiFiClient wifiClient;

void setup() {
  Serial.begin(115200);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(led4Pin, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(wifiClient, serverName);  // Updated to use WiFiClient
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(payload);

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      int led1 = doc["led1"];
      int led2 = doc["led2"];
      int led3 = doc["led3"];
      int led4 = doc["led4"];

      digitalWrite(led1Pin, led1);
      digitalWrite(led2Pin, led2);
      digitalWrite(led3Pin, led3);
      digitalWrite(led4Pin, led4);
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end();
  }

  delay(1000); // Check every 10 seconds
}

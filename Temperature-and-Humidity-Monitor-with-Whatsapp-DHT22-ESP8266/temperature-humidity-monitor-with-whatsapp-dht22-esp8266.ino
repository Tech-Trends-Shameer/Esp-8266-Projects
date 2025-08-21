#include <ESP8266WiFi.h>
#include <ThingESP.h>
#include <DHT.h>

#define DHTPIN 4       // GPIO2 (D2 pin on NodeMCU)
#define DHTTYPE DHT22  // Change to DHT11 if you are using DHT11 Sensor

DHT dht(DHTPIN, DHTTYPE);
ThingESP8266 thing("Username", "ProjectName", "ProjectUserName");

// Variables to store last readings
float lastTempC = 0;
float lastTempF = 0;
float lastHumid = 0;
unsigned long lastRead = 0;
const long READ_INTERVAL = 5000;  // Read every 5 seconds

void setup() {
  Serial.begin(115200);

  // Connect WiFi via ThingESP
  thing.SetWiFi("WiFi Username", "WiFi Password");
  thing.initDevice();

  // Initialize DHT sensor
  dht.begin();
  Serial.println("DHT22 Sensor initialized...");
}

void loop() {
  thing.Handle();

  unsigned long currentMillis = millis();
  if (currentMillis - lastRead > READ_INTERVAL) {
    lastRead = currentMillis;

    float h = dht.readHumidity();
    float tC = dht.readTemperature();
    float tF = dht.readTemperature(true);  // Fahrenheit

    if (!isnan(h) && !isnan(tC) && !isnan(tF)) {
      lastHumid = h;
      lastTempC = tC;
      lastTempF = tF;
      Serial.print("Temperature: ");
      Serial.print(lastTempC);
      Serial.print(" °C / ");
      Serial.print(lastTempF);
      Serial.print(" °F   Humidity: ");
      Serial.print(lastHumid);
      Serial.println(" %");
    } else {
      Serial.println("DHT22 read failed, keeping last values...");
    }
  }
}

String HandleResponse(String query) {
  if (query == "temperature") {
    return "Temperature: " + String(lastTempC) + " °C, " + String(lastTempF) + " °F";
  }
  else if (query == "humidity") {
    return "Humidity: " + String(lastHumid) + " %";
  }
  else {
    return "Your query was invalid..";
  }
}

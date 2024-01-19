//Tech Trends Shameer
//Air Quality Monitoring Using Blynk, ThingSpeak and ESP8266

#define BLYNK_TEMPLATE_ID "TMPL3_KL1ZjEs"
#define BLYNK_TEMPLATE_NAME "Air Quality Monitoring"
#define BLYNK_AUTH_TOKEN "jo71QKpxVdJeq2XSgduhJ2KodfuSq3ZhhdohvYDA"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MQ135.h"
#include <ESP8266WiFi.h>

String apiKey = "VU37fsMfJ5NGsdgfXEFNg5EQsg";  // Enter your Write API key from ThingSpeak
const char *ssid = "WiFi Username";   // replace with your WiFi SSID
const char *pass = "WiFi Password"; // replace with your WiFi password
const char *server = "api.thingspeak.com";
const int sensorPin = 0;
int air_quality;

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte degree_symbol[8] =
    {
        0b00111,
        0b00101,
        0b00111,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000};

char auth[] = BLYNK_AUTH_TOKEN;

BlynkTimer timer;

int gas = A0;
int sensorThreshold = 100;

#define DHTPIN 2     // Connect Out pin to D4 in NODE MCU
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void sendToThingSpeak(float temperature, float humidity, int airQuality)
{
  if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(temperature);
    postStr += "&field2=";
    postStr += String(humidity);
    postStr += "&field3=";
    postStr += String(airQuality);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" degrees Celsius, Humidity: ");
    Serial.print(humidity);
    Serial.print("%, Air Quality: ");
    Serial.print(airQuality);
    Serial.println(" PPM. Sent to ThingSpeak.");
  }
  client.stop();
}

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int analogSensor = analogRead(gas);

  Blynk.virtualWrite(V2, analogSensor);
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  Serial.print("Gas Value: ");
  Serial.println(analogSensor);
  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print("    Humidity : ");
  Serial.println(h);

  // Send data to ThingSpeak
  sendToThingSpeak(t, h, analogSensor);
}

void setup()
{
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(2000L, sendSensor);

  lcd.begin();
  lcd.createChar(1, degree_symbol);

  lcd.setCursor(3, 0);
  lcd.print("Air Quality");
  lcd.setCursor(3, 1);
  lcd.print("Monitoring");
  delay(1000);
  lcd.clear();

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop()
{
  Blynk.run();
  timer.run();

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasValue = analogRead(gas);

  lcd.setCursor(0, 0);
  lcd.print("Temperature ");
  lcd.setCursor(0, 1);
  lcd.print(t);
  lcd.setCursor(6, 1);
  lcd.write(1);
  lcd.setCursor(7, 1);
  lcd.print("C");
  delay(1000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Humidity ");
  lcd.print(h);
  lcd.print("%");
  delay(1000);
  lcd.clear();

  if (gasValue < 120)
  {
    lcd.setCursor(0, 0);
    lcd.print("Gas Value: ");
    lcd.print(gasValue);
    lcd.setCursor(0, 1);
    lcd.print("Fresh Air");
    Serial.println("Fresh Air");
    delay(1000);
    lcd.clear();
  }
  else if (gasValue > 120)
  {
    lcd.setCursor(0, 0);
    lcd.print("Gas Value: ");
    lcd.print(gasValue);
    lcd.setCursor(0, 1);
    lcd.print("Bad Air");
    Serial.println("Bad Air");
    delay(1000);
    lcd.clear();
  }

  if (gasValue > 120)
  {
    //Blynk.email("shameer50@gmail.com", "Alert", "Bad Air!");
    Blynk.logEvent("pollution_alert", "Bad Air");
  }
}

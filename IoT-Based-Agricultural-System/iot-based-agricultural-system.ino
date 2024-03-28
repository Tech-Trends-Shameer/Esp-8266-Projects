#define BLYNK_TEMPLATE_ID "TMPL3OSKg4doR"
#define BLYNK_TEMPLATE_NAME "IOT Based Agriculture System"
#define BLYNK_AUTH_TOKEN "fQrObEhb10fdjMGJbdfaIbsvfDjdwyUojqkyMJHfghj7HmTJ"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h> // Include the I2C LCD library

#define SOIL_MOISTURE_PIN A0
#define THRESHOLD_MOISTURE 10
#define PUMP_PIN D1
#define PUMP_SWITCH V6
#define DHT_PIN D2
#define DHT_TYPE DHT11
#define RAIN_SENSOR_PIN A0

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "WiFi Username";
char pass[] = "WiFi Password";

DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change the address to your LCD's address

BlynkTimer timer;

bool isPumpOn = false;

int temt6000Pin = A0;
float light;
int light_value;

void sendSensorData()
{
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  int soilMoisturePercentage = map(soilMoisture, 400, 1023, 100, 0);
  Serial.print("Soil Moisture Percentage ");
  Serial.println(soilMoisturePercentage);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.print("Temperature");
  Serial.println(temperature);
  Serial.print("Humidity");
  Serial.println(humidity);
  if (!isnan(temperature) && !isnan(humidity))
  {
    Blynk.virtualWrite(V0, temperature);
    Blynk.virtualWrite(V1, humidity);
  }
  else
  {
    Serial.println("Failed to read data from DHT sensor!");
  }
  Blynk.virtualWrite(V2, soilMoisturePercentage);

  int rainsensorValue = analogRead(RAIN_SENSOR_PIN);
  int rainValue = map(rainsensorValue, 400, 1023, 100, 0);
  
  Serial.print("Rain Sensor Value: ");
  Serial.println(rainValue);
  Blynk.virtualWrite(V4, rainValue); // Send rain sensor value to Blynk
  
  light_value = analogRead(temt6000Pin);
  light = light_value * 0.0976; // percentage calculation
  Serial.print("Light");
  Serial.println(light);
  //Blynk.virtualWrite(V3, light_value); // Send raw light value to Blynk
  Blynk.virtualWrite(V3, light);       // Send light percentage to Blynk
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil Moisture: ");
  lcd.print(soilMoisturePercentage);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
  lcd.setCursor(10, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 2);
  lcd.print("Rain Sensor: ");
  lcd.print(rainValue);
  lcd.print("   ");

  lcd.setCursor(0, 3);
  lcd.print("Light Intensity: ");
  lcd.print(light_value); // Display raw light value
  lcd.print(" | ");
  lcd.print(light); // Display light percentage

  
  if (isPumpOn || soilMoisturePercentage < THRESHOLD_MOISTURE || rainValue > 500)
  {
    digitalWrite(PUMP_PIN, HIGH);
    if (!isPumpOn)
    {
      Blynk.logEvent("moisture_alert", "Soil moisture is below the threshold!");
      Serial.println("Soil moisture is below the threshold!");
    }
  }
  else
  {
    if (!isPumpOn)
    {
      digitalWrite(PUMP_PIN, LOW);
    }
  }
}

BLYNK_WRITE(PUMP_SWITCH)
{
  isPumpOn = param.asInt();
  if (isPumpOn)
  {
    Serial.println("Pump manually turned ON");
  }
  else
  {
    Serial.println("Pump manually turned OFF");
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  dht.begin();

  lcd.begin(); // Initialize LCD
  //lcd.backlight();

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(3000L, sendSensorData);

  Blynk.virtualWrite(PUMP_SWITCH, isPumpOn);
  Blynk.syncVirtual(PUMP_SWITCH);
}

void loop()
{
  Blynk.run();
  timer.run();
}

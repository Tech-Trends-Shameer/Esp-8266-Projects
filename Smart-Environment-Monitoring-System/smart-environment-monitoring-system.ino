#define BLYNK_TEMPLATE_ID "TMPL3466yY8pl"
#define BLYNK_TEMPLATE_NAME "Environment Monitoring System"
#define BLYNK_AUTH_TOKEN "akZZG3qpc8k9_WkJ3xqx-zhIvGvLGKSh"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>

#include <DHT.h>


char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Galaxy F23 5G 3FF5";  // type your wifi name
char pass[] = "edrp6836ehan";  // type your wifi password

BlynkTimer timer;


#define DHTPIN 4 //Connect Out pin to D2 in NODE MCU
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

const int AirValue = 650;   //you need to replace this value with Value_1
const int WaterValue = 310;  //you need to replace this value with Value_2
const int SensorPin = A0;

void sendSensor()
{
  /*int soilmoisturevalue = analogRead(A0);
   soilmoisturevalue = map(soilmoisturevalue, 0, 1023, 0, 100);*/
   int value = analogRead(A0);
  //value = map(value,400,1023,100,0);
   value = map(value, AirValue, WaterValue, 0, 100);
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V0, value);
    Blynk.virtualWrite(V1, t);
    Blynk.virtualWrite(V2, h);
    Serial.print("Soil Moisture : ");
    Serial.print(value);
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);
}
void setup()
{   
  
  Serial.begin(115200);
  

  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(100L, sendSensor);
 
  }

void loop()
{
  Blynk.run();
  timer.run();
 }

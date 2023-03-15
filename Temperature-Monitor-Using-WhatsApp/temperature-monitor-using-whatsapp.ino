//Tech Trends Shameer
//Temperature Monitor Using Whatsapp

#include <ESP8266WiFi.h>
#include <ThingESP.h>
#include <DHT.h>

#define DHTPIN 2 
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);

ThingESP8266 thing("Username", "ProjectName", "ProjectUserName");

//int LED = D2;

unsigned long previousMillis = 0;
const long INTERVAL = 6000;  

void setup()
{
  Serial.begin(115200);

 // pinMode(LED, OUTPUT);

  thing.SetWiFi("WiFiUsername", "WiFiPassword");

  thing.initDevice();

}


String HandleResponse(String query)
{

 float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  Serial.println("Temperature :");
  Serial.print(t);
  Serial.println("Humidity");
  Serial.print(h);
  String humid = "Humidity: " + String(h) + " % \n";
  String temp = "Temperature: " + String(t) + "°C, " + String(f) + "°F \n";

  
  if (query == "temperature") {
   
    return temp;
  }

  else if (query == "humidity") {
  
    return humid;
  }
 
  else return "Your query was invalid..";

}


void loop()
{ 

  thing.Handle();

}
 
 

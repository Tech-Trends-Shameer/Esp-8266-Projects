//Tech Trends Shameer
//Temperature Monitor Using Whatsapp

#include <ESP8266WiFi.h>
#include <ThingESP.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp; // I2C

ThingESP8266 thing("techtrends", "ControlLED", "techtrends");

unsigned long previousMillis = 0;
const long INTERVAL = 6000;  

void setup()
{
  Serial.begin(115200);
   Serial.println(F("BMP280 test"));

  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */


  thing.SetWiFi("WIFI Username", "WIFI Password");

  thing.initDevice();

}


String HandleResponse(String query)
{
 float temp = bmp.readTemperature();
  float f = bmp.readTemperature()*1.8+32;
    float p = bmp.readPressure();
    float alt = bmp.readAltitude(1013.25);
    Serial.print(F("Temperature = "));
    Serial.print(temp);
    Serial.println(" *C");
    
    Serial.print(F("Pressure = "));
    Serial.print(p);
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(alt); /* Adjusted to local forecast! */
    Serial.println(" m");
 
  String pressure = "Pressure: " + String(p) + " pa \n";
  String altitude = "Altitude: " + String(alt) + " m \n";
  String temperature = "Temperature: " + String(temp) + "°C, " + String(f) + "°F \n";

  
  if (query == "temperature") {
   
    return temperature;
  }

  else if (query == "pressure") {
  
    return pressure;
  }
  else if (query == "altitude") {
  
    return altitude;
  }
 
  else return "Your query was invalid..";

}


void loop()
{ 

  thing.Handle();

}
 
 

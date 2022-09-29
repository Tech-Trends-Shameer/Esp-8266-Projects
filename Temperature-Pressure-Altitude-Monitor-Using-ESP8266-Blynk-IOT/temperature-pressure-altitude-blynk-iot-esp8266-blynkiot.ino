#define BLYNK_TEMPLATE_ID "TMPLfzdfdKVm-c6"
#define BLYNK_DEVICE_NAME "Temp Pressure and Altitude Monitor"
#define BLYNK_AUTH_TOKEN "LM6X0cjOSutdfdhhhwI7AnoYCwj0kLFLvZENGn"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
 

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);


char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "";  // type your wifi name
char pass[] = "";  // type your wifi password

BlynkTimer timer;

void sendSensor()
{

    float temp = bmp.readTemperature();
    float pressure = bmp.readPressure();
    float alt = bmp.readAltitude(1013.25);
   
    Serial.print(F("Temperature = "));
    Serial.print(temp);
    Serial.println(" *C");
   
    Serial.print(F("Pressure = "));
    Serial.print(pressure);
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(alt); /* Adjusted to local forecast! */
    Serial.println(" m");
 
 
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V0, temp);
    Blynk.virtualWrite(V1, pressure);
    Blynk.virtualWrite(V2, alt);
    Serial.print("Temperature : ");
    Serial.print(temp);
    Serial.print("    Pressure : ");
    Serial.print(pressure);
    Serial.print("    Altitude : ");
    Serial.println(alt);
}
void setup()
{   
  
  Serial.begin(115200);
  

  Blynk.begin(auth, ssid, pass);
  //dht.begin();
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

  timer.setInterval(100L, sendSensor);
 
  }

void loop()
{
  Blynk.run();
  timer.run();
 }

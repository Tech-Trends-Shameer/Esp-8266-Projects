#define BLYNK_PRINT Serial 

#define BLYNK_TEMPLATE_ID "TMPL3hHougpcA"
#define BLYNK_TEMPLATE_NAME "Patient Health Monitoring"
#define BLYNK_AUTH_TOKEN "XONsdf3XXdNdsfp7BPv-58bgdftihYRgfh42l1fgj7Zp4w"

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
 
#define REPORTING_PERIOD_MS 1000
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTPIN 2 // Pin which is connected to the DHT sensor.
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

char auth[] = BLYNK_AUTH_TOKEN;             // You should get Auth Token in the Blynk App.
char ssid[] = "WiFi Username";                                     // Your WiFi credentials.
char pass[] = "WiFi Password";
 
// Connections : SCL PIN - D22 , SDA PIN - D21 , INT PIN - D0
PulseOximeter pox;
 
int BPM, SpO2, humidity, temperature;;
uint32_t tsLastReport = 0;
 
 
void onBeatDetected()
{
    Serial.println("Beat Detected!");
}
 
void setup()
{
    Serial.begin(115200);
    
    pinMode(19, OUTPUT);
    Blynk.begin(auth, ssid, pass);
 
    Serial.print("Initializing Pulse Oximeter..");
    lcd.begin();
  lcd.backlight();
  
  // Display "Tech Trends Shameer"
  lcd.setCursor(3, 0);
  lcd.print("Tech Trends ");
  lcd.setCursor(3, 1);
  lcd.print("  Shameer    ");
  delay(2000);  // Wait for 3 seconds

  // Clear the LCD and display "Pulse Oximeter"
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Patient Health ");
  lcd.setCursor(3, 1);
  lcd.print("Monitoring   ");
  lcd.setCursor(12, 0);  // Adjust the position for the heart symbol 
  
  delay(2000);
lcd.clear();
 
    if (!pox.begin())
    {
         Serial.println("FAILED");
         for(;;);
    }
    else
    {
         Serial.println("SUCCESS");
         pox.setOnBeatDetectedCallback(onBeatDetected);
    }
 
    // The default current for the IR LED is 50mA and it could be changed by uncommenting the following line.
        pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
}
 
void loop()
{
    pox.update();
    Blynk.run();
 
    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    


    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        //lcd.clear();
        lcd.setCursor(0 , 0);
        lcd.print("BPM:");
        lcd.print(pox.getHeartRate());
        //lcd.setCursor(10, 0);  // Adjust the position for the heart symbol 
        lcd.setCursor(0 , 1);
        lcd.print("Sp02:");
        lcd.print(pox.getSpO2());
        lcd.print("%");
        //tsLastReport = millis();

        Serial.print("Heart rate:");
        Serial.print(BPM);
        Serial.print(" bpm / SpO2:");
        Serial.print(SpO2);
        Serial.println(" %");
 
        
        //tsLastReport = millis();       

   
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print(" %\t");
        Serial.print("Temp: ");
        Serial.print(temperature);
        Serial.println(" *C");
        //lcd.clear();
        lcd.setCursor(9 , 0);
        lcd.print("T: ");
        lcd.print(temperature);
        lcd.print((char)223);
        lcd.print("C");
        //lcd.setCursor(10, 0);  // Adjust the position for the heart symbol 
        lcd.setCursor(9 , 1);
        lcd.print("H: ");
        lcd.print(humidity);
        lcd.print("%  ");
        Blynk.virtualWrite(V1, BPM);
        Blynk.virtualWrite(V2, SpO2); 
        Blynk.virtualWrite(V3, temperature);
        Blynk.virtualWrite(V4, humidity);
	      tsLastReport = millis();
	   

    }
    
}

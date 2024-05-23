#define BLYNK_TEMPLATE_ID "TMPL31kVdgJmv"
#define BLYNK_TEMPLATE_NAME "Noise Pollution Monitoring"
#define BLYNK_AUTH_TOKEN "dp0J9sdfNlZkPVgaS4dgilKCgzpRSMgYfAgddX-lg"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#define SENSOR_PIN A0
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int sampleWindow = 50;
unsigned int sample;
int db;
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "WiFi Username";
char pass[] = "WiFi Password";
//BLYNK_READ(V0)
//{
 
//}
void setup() {
   Serial.begin(115200);
  pinMode (SENSOR_PIN, INPUT);
  lcd.begin();
  lcd.backlight();

  // Display "Tech Trends Shameer"
  lcd.setCursor(3, 0);
  lcd.print("Tech Trends ");
  lcd.setCursor(3, 1);
  lcd.print("  Shameer    ");
  delay(2000);  // Wait for 3 seconds

  // Clear the LCD and display "Noise Pollution Monitoring"
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Noise Pollution ");
  lcd.setCursor(3, 1);
  lcd.print("Monitoring   ");
   
  
  delay(2000);
lcd.clear();

  lcd.clear();
  Blynk.begin(auth, ssid, pass);
  Blynk.virtualWrite(V0, db);
}
void loop() {
  Blynk.run();
  unsigned long startMillis = millis();  // Start of sample window
  float peakToPeak = 0;  // peak-to-peak level
  unsigned int signalMax = 0;  //minimum value
  unsigned int signalMin = 1024;  //maximum value
  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(SENSOR_PIN);  //get reading from microphone
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  Serial.println(peakToPeak);
  db = map(peakToPeak, 20, 900, 0, 90);  //calibrate for deciBels
  //  db = map(peakToPeak, 20, 900, 49.5, 90);  //calibrate for deciBels
 // Blynk.virtualWrite(V0, db);
  lcd.setCursor(0, 0);
  lcd.print("Loudness: ");
  lcd.print(db);
  lcd.print("dB");
  if (db <= 50)
  {
    lcd.setCursor(0, 1);
    lcd.print("Level: Quite");
    Serial.println("Level: Quite");
    Serial.println(db);
  }
  else if (db > 50 && db < 75)
  {
    lcd.setCursor(0, 1);
    lcd.print("Level: Moderate");
    Serial.println("Level: Moderate");
    Serial.println(db);
  }
  else if (db >= 75)
  {
    lcd.setCursor(0, 1);
    lcd.print("Level: High");
    Serial.println("Level: High");
    Serial.println(db);
  }
  delay(600);
  Blynk.virtualWrite(V0, db);
  lcd.clear();
}

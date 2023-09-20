//Tech Trends Shameer

#define BLYNK_TEMPLATE_ID "TMPL3NKgjZWZjkfyHo"
#define BLYNK_TEMPLATE_NAME "Water Level Monitoring"
#define BLYNK_AUTH_TOKEN "3CX-aksZvi_XsAdfghjjJKStOW_ZuX_UG_VNcJ"

#include <ESP8266WiFi.h> 
#include <WiFiClient.h>

#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
//#include <BlynkTimer.h> // Added BlynkTimer library

LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = BLYNK_AUTH_TOKEN; // Enter your Blynk authentication token

char ssid[] = "WiFi Username"; // Enter your WiFi SSID
char pass[] = "WiFi Password"; // Enter your WiFi password

BlynkTimer timer;
//#define trig 13
//#define echo 12
const int trig = D5;   
 const int echo = D6;  
int MaxLevel = 18;

int Level1 = (MaxLevel * 75) / 100;
int Level2 = (MaxLevel * 65) / 100;
int Level3 = (MaxLevel * 55) / 100;
int Level4 = (MaxLevel * 45) / 100;
int Level5 = (MaxLevel * 35) / 100;

void setup() {
  Serial.begin(9600);
  lcd.begin(); // Initialize the LCD with correct column and row count
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  Serial.print("LCD 1");
  lcd.setCursor(3, 0);  //Set cursor to first column of first row
  lcd.print("Tech Trends");
   Serial.print("LCD 2");
  delay(3000);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Water level");
  lcd.setCursor(3, 1);
  lcd.print("Monitoring");
  delay(7000);
  lcd.clear();

  // Call the functions
  timer.setInterval(100L, ultrasonic);
}

void loop() {
  Blynk.run();
  timer.run(); // Run the BlynkTimer
  delay(1000);
}

void ultrasonic() {
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long t = pulseIn(echo, HIGH);
  int distance = t / 29 / 2;

  int blynkDistance = (distance - MaxLevel) * -1;
  if (distance <= MaxLevel) {
    Blynk.virtualWrite(V0, blynkDistance);
    Serial.print("Distance: ");
    Serial.print(blynkDistance);
    Serial.println(" cm");
  } else {
    Blynk.virtualWrite(V0, 0);
  }

  lcd.setCursor(0, 0);
  lcd.print("Level:");

  if (Level1 <= distance) {
    lcd.setCursor(8, 0); // Corrected column position
    lcd.print("Very Low");
  } else if (Level2 <= distance && Level1 > distance) {
    lcd.setCursor(8, 0); // Corrected column position
    lcd.print("Low");
    lcd.print("      ");
  } else if (Level3 <= distance && Level2 > distance) {
    lcd.setCursor(8, 0); // Corrected column position
    lcd.print("Medium");
    lcd.print("      ");
  } else if (Level4 <= distance && Level3 > distance) {
    lcd.setCursor(8, 0); // Corrected column position
    lcd.print("High");
    lcd.print("      ");
  } else if (Level5 >= distance) {
    lcd.setCursor(8, 0); // Corrected column position
    lcd.print("Full");
    lcd.print("      ");
  }
}

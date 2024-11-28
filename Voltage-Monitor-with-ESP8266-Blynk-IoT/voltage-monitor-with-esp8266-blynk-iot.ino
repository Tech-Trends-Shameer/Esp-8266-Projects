#define BLYNK_TEMPLATE_ID "TMPL3M4dEmGdb"
#define BLYNK_TEMPLATE_NAME "Voltage Measurement"
#define BLYNK_AUTH_TOKEN "4kKknYffAu88domL0W6HlrJ8H-b4tDtstYTjK9"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>

// LCD configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Calibration factor and offset
const float vCalibration = 175.0;
const float offsetVoltage = 0.522; // Observed baseline voltage when power is OFF

// Variables
int analogValue = 0;
float voltage = 0.0;

// Wi-Fi credentials
 
const char* ssid = "WiFi Username"; // Your WiFi network SSID
const char* pass = "WiFi Password"; // Your WiFi network password

// Blynk authentication
char auth[] = BLYNK_AUTH_TOKEN;

// Blynk timer
BlynkTimer timer;

// Function to send voltage to Blynk
void sendVoltageToBlynk(float actualVoltage) {
  Blynk.virtualWrite(V1, actualVoltage); // Send voltage to virtual pin V1
  Serial.print("Sent to Blynk: ");
  Serial.println(actualVoltage);
}

// Function to get a stable voltage reading
float getStableVoltage() {
  const int sampleCount = 10; // Number of samples for averaging
  float totalVoltage = 0.0;

  // Take multiple readings
  for (int i = 0; i < sampleCount; i++) {
    analogValue = analogRead(A0);
    voltage = (analogValue / 1024.0) * 1.0; // Convert to 0-1V range
    totalVoltage += voltage;
    delay(20); // Short delay between samples
  }

  // Calculate average voltage
  return totalVoltage / sampleCount;
}

void setup() 
{
  Serial.begin(115200);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Voltage Sensor");
  delay(2000);
  lcd.clear();

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");

  // Connect to Blynk
  Blynk.begin(auth, ssid, pass);
  Serial.println("Connected to Blynk!");

  // Set up a timer to send voltage data to Blynk every 2 seconds
  timer.setInterval(2000L, []() {
    float stableVoltage = getStableVoltage();
    float actualVoltage = (stableVoltage - offsetVoltage) * vCalibration;
    if (actualVoltage < 1) actualVoltage = 0;

    sendVoltageToBlynk(actualVoltage);

    // Update LCD
    lcd.clear();
    if (actualVoltage > 0) {
      lcd.setCursor(0, 0);
      lcd.print("Voltage: ");
      lcd.print(actualVoltage, 2);
      lcd.print(" V");
    } else 
     if(actualVoltage < 1)
    {
      lcd.setCursor(0, 0);
      lcd.print("Power OFF");
    }
  });
}

void loop() {
  Blynk.run();
  timer.run();
}

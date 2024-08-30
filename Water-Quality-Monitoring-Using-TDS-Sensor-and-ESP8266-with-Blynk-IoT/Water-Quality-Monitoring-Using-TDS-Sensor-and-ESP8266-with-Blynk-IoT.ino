#define BLYNK_TEMPLATE_ID "TMPL3IBzVEG9V"
#define BLYNK_TEMPLATE_NAME "Water Quality Monitoring"
#define BLYNK_AUTH_TOKEN "KJRzl2jRdsfcIPL5XD1aHMAhY5RjbRjyhgfBiWO-"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

#include <WiFiClient.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WiFi Username";
char pass[] = "WiFi Password";

char auth[] = BLYNK_AUTH_TOKEN;

namespace pin {
    const byte tds_sensor = A0;
}

namespace device {
    float aref = 3.3; // Vref, this is for 3.3v compatible controller boards, for Arduino use 5.0v.
}

namespace sensor {
    float ec = 0;
    unsigned int tds = 0;
    float ecCalibration = 1;
}

void setup() {
    Serial.begin(115200); // Debugging on hardware Serial 0
    Blynk.begin(auth, ssid, pass);
}

void loop() {
    Blynk.run();
    readTdsQuick();
    delay(1000);
}

void readTdsQuick() {
    // Read the raw analog value and convert to voltage
    float rawEc = analogRead(pin::tds_sensor) * device::aref / 1024.0;
    
    // Debugging: Print the raw analog value
    Serial.print(F("Raw Analog Value: "));
    Serial.println(rawEc);

    // Adjust this offset based on the sensor's dry reading (without immersion)
    float offset = 0.14; // Set this to the observed raw analog value in air

    // Apply calibration and offset compensation
    sensor::ec = (rawEc * sensor::ecCalibration) - offset;

    // If the EC is below zero after adjustment, set it to zero
    if (sensor::ec < 0) sensor::ec = 0;

    // Convert voltage value to TDS value using a cubic equation
    sensor::tds = (133.42 * pow(sensor::ec, 3) - 255.86 * sensor::ec * sensor::ec + 857.39 * sensor::ec) * 0.5;

    // Debugging: Print the TDS and EC values
    Serial.print(F("TDS: "));
    Serial.println(sensor::tds);
    Serial.print(F("EC: "));
    Serial.println(sensor::ec, 2);

    // Send data to Blynk virtual pins
    Blynk.virtualWrite(V0, sensor::tds);
    Blynk.virtualWrite(V1, sensor::ec);
}

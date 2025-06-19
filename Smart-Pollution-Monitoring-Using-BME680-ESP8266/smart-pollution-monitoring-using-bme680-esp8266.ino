// Tech Trends Shameer
// IoT-Based Pollution Monitoring System Using ESP32, BME680, and Blynk
#define BLYNK_TEMPLATE_ID "TMPL3TDu4i0eC"
#define BLYNK_TEMPLATE_NAME "Distance Measurement"
#define BLYNK_AUTH_TOKEN "7JNt-fFQrRJ-s5H13KiTnhKkDjpcMnHnSlJQ3"

#define BLYNK_PRINT Serial
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "WiFi Username";
char pass[] = "WiFi Password";

// BME680 Sensor
Adafruit_BME680 bme;

// Blynk Timer
BlynkTimer timer;

void setup() {
    Serial.begin(115200);

    // Connect to WiFi
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi!");

    // Initialize Blynk
    Blynk.begin(auth, ssid, pass);

    // Initialize BME680
    if (!bme.begin()) {
        Serial.println("Could not find a valid BME680 sensor!");
        while (1);
    }

    // Set sensor settings
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setGasHeater(320, 150); // 320°C for 150ms

    // Call sendSensorData() every 10 seconds
    timer.setInterval(10000L, sendSensorData);
}

// Function to send sensor data to Blynk
void sendSensorData() {
    if (!bme.performReading()) {
        Serial.println("Failed to read BME680 sensor!");
        return;
    }

    float temp = bme.temperature;
    float hum = bme.humidity;
    float press = bme.pressure / 100.0; // Convert to hPa
    float air_quality = bme.gas_resistance / 1000.0; // Convert to kOhms

    Serial.print("Temperature: "); Serial.print(temp); Serial.println("°C");
    Serial.print("Humidity: "); Serial.print(hum); Serial.println("%");
    Serial.print("Pressure: "); Serial.print(press); Serial.println("hPa");
    Serial.print("Air Quality (VOC): "); Serial.print(air_quality); Serial.println(" kOhms");

    // Send data to Blynk
    Blynk.virtualWrite(V0, temp);
    Blynk.virtualWrite(V1, hum);
    Blynk.virtualWrite(V2, press);
    Blynk.virtualWrite(V3, air_quality);
}

void loop() {
    Blynk.run();
    timer.run();  // Ensures the sendSensorData() function runs every 10 seconds
}

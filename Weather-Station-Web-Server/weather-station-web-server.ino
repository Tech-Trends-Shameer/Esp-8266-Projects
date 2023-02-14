//Tech Trends Shameer
//Weather Station Project

// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include "DHT.h"

//Uncomment the line based on the sensor you are using.
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
               
//Enter your wifi username and wifi password below
const char* ssid = "";
const char* password = "";

// Web Server on port 80
WiFiServer server(80);

// DHT Sensor
const int DHTPin = 5;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];
 

// only runs once on boot
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);

  dht.begin();
  
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
}

// runs over and over again
void loop() {
  // Listenning for new clients
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && blank_line) {            
            float h = dht.readHumidity();
            // Read temperature as Celsius (the default)
            float t = dht.readTemperature();
            // Read temperature as Fahrenheit (isFahrenheit = true)
            float f = dht.readTemperature(true);
            // Check if any reads failed and exit early (to try again).
            if (isnan(h) || isnan(t) || isnan(f)) {
              Serial.println("Failed to read from DHT sensor!");
              strcpy(celsiusTemp,"Failed");
              strcpy(fahrenheitTemp, "Failed");
              strcpy(humidityTemp, "Failed");         
            }
            else{
              // Computes temperature values in Celsius + Fahrenheit and Humidity
              float hic = dht.computeHeatIndex(t, h, false);       
              dtostrf(hic, 6, 2, celsiusTemp);             
              float hif = dht.computeHeatIndex(f, h);
              dtostrf(hif, 6, 2, fahrenheitTemp);         
              dtostrf(h, 6, 2, humidityTemp);
              // The below lines are just need to print in Serial Monitor. You can delete if its not needed.
              Serial.print("Humidity: ");
              Serial.print(" % ");
              Serial.println(h);
              Serial.println("Temperature: ");
              Serial.print(t);
              Serial.print(" *C ");
              Serial.print(f);
              Serial.println(" *F ");
              Serial.println("Heat index: ");
              Serial.print(hic);
              Serial.print(" *C ");
              Serial.print(hif);
              Serial.println(" *F");
            }
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            // Below lines are to display the details in the web page  
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head></head><body><h1>Weather Station by Tech Trends</h1><h3>Temperature in Celsius: ");
            client.println(t);
            client.print(char(176)); //To print the degree symbol
            client.println(" C</h3><h3>Temperature in Fahrenheit: ");
            client.println(f);
            client.print(char(176)); //To print the degree symbol
            client.println(" F</h3><h3>Humidity: ");
            client.println(humidityTemp);
            client.println("%</h3><h3>Heat Index in Celsius: ");
            float hic = dht.computeHeatIndex(t, h, false);        
            client.println(hic);
            client.print(char(176)); //To print the degree symbol
            client.println(" C</h3><h3>Heat Index in Fahrenheit: ");
            float hif = dht.computeHeatIndex(f, h);
            client.println(hif);
            client.print(char(176)); //To print the degree symbol
            client.println(" F</h3><h3>");
            client.println("</body></html>");     
            break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }  
    // Closing the connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
}   

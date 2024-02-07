//Tech Trends Shameer
//Home Automation Web Server with ESP8266

#include <ESP8266WiFi.h>
 
// Replace with your network credentials
const char* ssid = "WiFi Username";
const char* password = "WiFi Password";
 
// Set web server port number to 80
WiFiServer server(80);
 
// Variable to store the HTTP request
String header;
 
// Auxiliar variables to store the current output state
String output0State = "off";
String output1State = "off";
String output2State = "off";
String output3State = "off";
 
// Assign output variables to GPIO pins
const int output0 = D1;     //GPIO4
const int output1 = D2;     //GPIO14
const int output2 = D3;     //GPIO12
const int output3 = D4;     //GPIO5
 
void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output0, OUTPUT);
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output3, OUTPUT);
  
  // Set outputs to LOW
  digitalWrite(output0, LOW);
  digitalWrite(output1, LOW);
  digitalWrite(output2, LOW);
  digitalWrite(output3, LOW);
 
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
 
void loop() 
{
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) 
  {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
 
            // turns the GPIOs on and off
            if (header.indexOf("GET /4/on") >= 0) 
            {
              Serial.println("GPIO 4 on");
              output0State = "on";
              digitalWrite(output0, HIGH);
            } 
            else if (header.indexOf("GET /4/off") >= 0) 
            {
              Serial.println("GPIO 4 off");
              output0State = "off";
              digitalWrite(output0, LOW);
            } 
 
 
            // turns the GPIOs on and off
            else if (header.indexOf("GET /14/on") >= 0) 
            {
              Serial.println("GPIO 14 on");
              output1State = "on";
              digitalWrite(output1, HIGH);
            } 
            else if (header.indexOf("GET /14/off") >= 0) 
            {
              Serial.println("GPIO 14 off");
              output1State = "off";
              digitalWrite(output1, LOW);
            }
 
 
            // turns the GPIOs on and off
            else if (header.indexOf("GET /12/on") >= 0) 
            {
              Serial.println("GPIO 12 on");
              output2State = "on";
              digitalWrite(output2, HIGH);
            } 
            else if (header.indexOf("GET /12/off") >= 0) 
            {
              Serial.println("GPIO 12 off");
              output2State = "off";
              digitalWrite(output2, LOW);
            }
 
 
            // turns the GPIOs on and off
            else if (header.indexOf("GET /5/on") >= 0) 
            {
              Serial.println("GPIO 5 on");
              output3State = "on";
              digitalWrite(output3, HIGH);
            } 
            else if (header.indexOf("GET /5/off") >= 0) 
            {
              Serial.println("GPIO 5 off");
              output3State = "off";
              digitalWrite(output3, LOW);
            }
            
 
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #6aa84f; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #93c47d;}</style></head>");
 
            // Web Page Heading
            client.println("<body><h1>Home Automation Web Server</h1>");
             //client.println("<body><h2>using Local Web Server</h2>");
 
            // Display current state, and ON/OFF buttons for GPIO 4
            client.println("<p>Bulb 1 - State " + output0State + "</p>");
            // If the output0State is off, it displays the ON button
            if (output0State == "off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
 
 
            // Display current state, and ON/OFF buttons for GPIO 14
            client.println("<p>Bulb 2 - State " + output1State + "</p>");
            // If the output1State is off, it displays the ON button
            if (output1State == "off") {
              client.println("<p><a href=\"/14/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/14/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
 
            // Display current state, and ON/OFF buttons for GPIO 12
            client.println("<p>Bulb 3 - State " + output2State + "</p>");
            // If the output2State is off, it displays the ON button
            if (output2State == "off") {
              client.println("<p><a href=\"/12/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/12/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
 
 
            // Display current state, and ON/OFF buttons for GPIO 5
            client.println("<p>Bulb 4 - State " + output3State + "</p>");
            // If the output3State is off, it displays the ON button
            if (output3State == "off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
 
 
            client.println("</body></html>");
 
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

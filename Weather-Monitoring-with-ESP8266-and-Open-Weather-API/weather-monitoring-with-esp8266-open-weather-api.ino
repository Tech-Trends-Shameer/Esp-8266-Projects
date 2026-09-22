// ESP8266 Wi-Fi Weather Station — Web Form Input Version
// Type a city name into a webpage hosted by the ESP8266 itself.
// Shows Temperature, Condition, and Humidity on the webpage AND on the 16x2 I2C LCD.
// Uses OpenWeatherMap (plain HTTP, no SSL needed — free tier).

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>   // Install "LiquidCrystal I2C" by Frank de Brabander

LiquidCrystal_I2C lcd(0x27, 16, 2);   // change to 0x3F if your module uses that address
ESP8266WebServer server(80);

const char* ssid = "WiFi Username";
const char* password = "WiFi Password";
const char* apiKey = "Open Weather API";


String lastResultHTML = "";

// ---- LCD continuous-cycle state (non-blocking, matches web result) ----
bool weatherAvailable = false;
String lcdCity = "";
String lcdTemp = "";
String lcdCondition = "";
String lcdHumidity = "";

int lcdScreenIndex = 0;              // 0=City 1=Temperature 2=Condition 3=Humidity
unsigned long lastLCDSwitch = 0;
const unsigned long LCD_SCREEN_TIME = 2500;   // ms per screen

void setup()
{
  Serial.begin(115200);
  delay(500);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts > 40)
    {
      Serial.println("\nFailed to connect. Restarting...");
      lcd.clear();
      lcd.print("WiFi Failed");
      delay(2000);
      ESP.restart();
    }
  }

  Serial.println("\nWiFi Connected");
  Serial.print("Open this in your browser: http://");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IP:");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP().toString());
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tech Trends");
  lcd.setCursor(0, 1);
  lcd.print("Shameer");
  delay(1500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter city on");
  lcd.setCursor(0, 1);
  lcd.print("web page...");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/weather", HTTP_GET, handleWeather);
  server.begin();
  Serial.println("Web server started.");
}

void loop()
{
  server.handleClient();
  updateLCDCycle();   // non-blocking — keeps rotating screens without freezing the web server
}

// ---------------- LCD CONTINUOUS CYCLE ----------------
void updateLCDCycle()
{
  if (!weatherAvailable) return;

  if (millis() - lastLCDSwitch >= LCD_SCREEN_TIME)
  {
    lastLCDSwitch = millis();
    lcdScreenIndex = (lcdScreenIndex + 1) % 4;
    showLCDScreen(lcdScreenIndex);
  }
}

void showLCDScreen(int index)
{
  lcd.clear();
  switch (index)
  {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("City:");
      lcd.setCursor(0, 1);
      lcd.print(lcdCity.substring(0, 16));
      break;
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("Temperature:");
      lcd.setCursor(0, 1);
      lcd.print(lcdTemp);
      break;
    case 2:
      lcd.setCursor(0, 0);
      lcd.print("Condition:");
      lcd.setCursor(0, 1);
      lcd.print(lcdCondition.substring(0, 16));
      break;
    case 3:
      lcd.setCursor(0, 0);
      lcd.print("Humidity:");
      lcd.setCursor(0, 1);
      lcd.print(lcdHumidity);
      break;
  }
}

// ---------------- WEB PAGE ----------------
void handleRoot()
{
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Tech Trends Shameer | Weather Station</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    * { box-sizing: border-box; }
    body {
      font-family: 'Segoe UI', Arial, sans-serif;
      background: linear-gradient(135deg, #4facfe 0%, #00f2fe 50%, #43e97b 100%);
      text-align: center;
      padding-top: 30px;
      margin: 0;
      min-height: 100vh;
    }
    .brand {
      color: #fff;
      font-size: 15px;
      font-weight: 600;
      letter-spacing: 1px;
      text-shadow: 0 1px 3px rgba(0,0,0,0.25);
      margin-bottom: 6px;
    }
    .brand span { font-size: 18px; }
    .card {
      background: #ffffff;
      max-width: 360px;
      margin: 10px auto;
      padding: 25px;
      border-radius: 18px;
      box-shadow: 0 8px 24px rgba(0,0,0,0.2);
    }
    h2 {
      color: #222;
      margin-top: 0;
      margin-bottom: 18px;
    }
    input {
      width: 90%;
      padding: 12px;
      margin: 8px 0;
      border-radius: 8px;
      border: 2px solid #4facfe;
      font-size: 16px;
      outline: none;
    }
    input:focus { border-color: #00c9ff; }
    button {
      background: linear-gradient(135deg, #f7971e, #ffd200);
      color: #333;
      font-weight: bold;
      padding: 12px 26px;
      border: none;
      border-radius: 8px;
      font-size: 16px;
      cursor: pointer;
      margin-top: 8px;
      box-shadow: 0 4px 10px rgba(0,0,0,0.15);
    }
    button:hover { opacity: 0.9; }
    .result {
      margin-top: 22px;
      padding: 18px;
      border-radius: 12px;
      background: linear-gradient(135deg, #e0f7fa, #e1f5fe);
      font-size: 16px;
      color: #0d47a1;
      text-align: left;
      line-height: 1.7;
      font-weight: 600;
    }
    .footer {
      color: #fff;
      font-size: 13px;
      margin-top: 18px;
      text-shadow: 0 1px 3px rgba(0,0,0,0.25);
      padding-bottom: 20px;
    }
  </style>
</head>
<body>
  <div class="brand"><span>📡</span> Tech Trends Shameer</div>
  <div class="card">
    <h2>🌦️ Weather Station</h2>
    <form action="/weather" method="GET">
      <input type="text" name="city" placeholder="Enter city e.g. Mumbai" required><br>
      <button type="submit">Get Weather 🔍</button>
    </form>
    <div class="result">RESULT_PLACEHOLDER</div>
  </div>
  <div class="footer">ESP8266 IoT Project • Tech Trends Shameer</div>
</body>
</html>
)rawliteral";

  html.replace("RESULT_PLACEHOLDER", lastResultHTML);
  server.send(200, "text/html; charset=utf-8", html);
}

// ---------------- HANDLE FORM SUBMIT ----------------
void handleWeather()
{
  if (!server.hasArg("city"))
  {
    lastResultHTML = "Missing city name.";
    server.sendHeader("Location", "/");
    server.send(303);
    return;
  }

  String city = server.arg("city");
  city.trim();

  if (city.length() == 0)
  {
    lastResultHTML = "Please enter a city.";
    lcdStatusMessage("Empty city!");
    server.sendHeader("Location", "/");
    server.send(303);
    return;
  }

  lcdStatusMessage("Fetching...");
  lastResultHTML = getWeather(city);

  server.sendHeader("Location", "/");
  server.send(303);
}

// ---------------- WEATHER FETCH ----------------
String getWeather(String city)
{
  WiFiClient client;
  HTTPClient http;

  String queryCity = city;
  queryCity.replace(" ", "%20");   // URL-encode spaces for the API call only

  String url = "http://api.openweathermap.org/data/2.5/weather?q=" + queryCity +
               "&appid=" + String(apiKey) + "&units=metric";

  http.begin(client, url);
  int code = http.GET();

  if (code <= 0)
  {
    Serial.println("HTTP request failed. Code: " + String(code));
    weatherAvailable = false;
    lcdStatusMessage("Request failed");
    http.end();
    return "Request failed.";
  }

  String payload = http.getString();
  http.end();

  DynamicJsonDocument doc(2048);
  DeserializationError err = deserializeJson(doc, payload);

  if (err)
  {
    weatherAvailable = false;
    lcdStatusMessage("Parse error");
    return "JSON parse error.";
  }

  int cod = doc["cod"].as<int>();
  if (cod != 200)
  {
    const char* msg = doc["message"] | "City not found";
    weatherAvailable = false;
    lcdStatusMessage("City not found");
    return "Error: " + String(msg);
  }

  // Use the API's own "name" field — correctly capitalized (e.g. "Mumbai")
  String properCity = doc["name"].as<String>();
  float temp = doc["main"]["temp"];
  int humidity = doc["main"]["humidity"];
  String description = doc["weather"][0]["description"].as<String>();

  Serial.println("\n--- WEATHER ---");
  Serial.println("City: " + properCity);
  Serial.println("Temp: " + String(temp, 1) + " C");
  Serial.println("Condition: " + description);
  Serial.println("Humidity: " + String(humidity) + "%");

  // ---- Set LCD cycle data (shown continuously by updateLCDCycle in loop()) ----
  lcdCity = properCity;
  lcdTemp = String(temp, 1) + (char)223 + "C";
  lcdCondition = description;
  lcdHumidity = String(humidity) + "%";
  weatherAvailable = true;

  // Show the first screen immediately instead of waiting for the timer
  lcdScreenIndex = 0;
  lastLCDSwitch = millis();
  showLCDScreen(lcdScreenIndex);

  // Weather emoji for the web result
  String icon = "🌤️";
  String descLower = description;
  descLower.toLowerCase();
  if (descLower.indexOf("rain") >= 0) icon = "🌧️";
  else if (descLower.indexOf("cloud") >= 0) icon = "☁️";
  else if (descLower.indexOf("clear") >= 0) icon = "☀️";
  else if (descLower.indexOf("snow") >= 0) icon = "❄️";
  else if (descLower.indexOf("thunder") >= 0) icon = "⛈️";
  else if (descLower.indexOf("mist") >= 0 || descLower.indexOf("fog") >= 0 || descLower.indexOf("haze") >= 0) icon = "🌫️";

  String resultText = icon + " <b>" + properCity + "</b><br>" +
                       "🌡️ Temperature: " + String(temp, 1) + " &deg;C<br>" +
                       "📋 Condition: " + description + "<br>" +
                       "💧 Humidity: " + String(humidity) + "%";
  return resultText;
}

void lcdStatusMessage(String msg)
{
  weatherAvailable = false;   // pause cycling while showing a status message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(msg.substring(0, 16));
}



// ESP8266 Wi-Fi Smart Dictionary — Web Form Input Version
// Type a word into a webpage hosted by the ESP8266 itself, choose Meaning/Synonym/Antonym.
// Result shows on the webpage AND on the 16x2 I2C LCD (auto-scrolling).

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>   // Install "LiquidCrystal I2C" by Frank de Brabander

LiquidCrystal_I2C lcd(0x27, 16, 2);   // change to 0x3F if your module uses that address
ESP8266WebServer server(80);

const char* ssid = "WiFi Username";
const char* password = "WiFi Password";

String lastResultHTML = "";   // holds the last lookup result to show on the page

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

  server.on("/", HTTP_GET, handleRoot);
  server.on("/lookup", HTTP_GET, handleLookup);
  server.begin();
  Serial.println("Web server started.");
}

void loop()
{
  server.handleClient();
}

// ---------------- WEB PAGE ----------------
void handleRoot()
{
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP8266 Smart Dictionary</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial, sans-serif; background:#f2f2f2; text-align:center; padding-top:40px; }
    .card { background:#fff; max-width:340px; margin:auto; padding:25px; border-radius:12px;
             box-shadow:0 2px 8px rgba(0,0,0,0.15); }
    h2 { color:#333; }
    input, select { width:90%; padding:10px; margin:8px 0; border-radius:6px; border:1px solid #ccc; font-size:16px; }
    button { background:#1565c0; color:white; padding:10px 20px; border:none; border-radius:6px;
             font-size:16px; cursor:pointer; margin-top:10px; }
    button:hover { background:#0d47a1; }
    .result { margin-top:20px; font-size:16px; color:#1565c0; font-weight:bold; text-align:left; }
  </style>
</head>
<body>
  <div class="card">
    <h2>Smart Dictionary</h2>
    <form action="/lookup" method="GET">
      <input type="text" name="word" placeholder="Enter a word" required><br>
      <select name="mode">
        <option value="meaning">Meaning</option>
        <option value="synonym">Synonym</option>
        <option value="antonym">Antonym</option>
      </select><br>
      <button type="submit">Search</button>
    </form>
    <div class="result">RESULT_PLACEHOLDER</div>
  </div>
</body>
</html>
)rawliteral";

  html.replace("RESULT_PLACEHOLDER", lastResultHTML);
  server.send(200, "text/html", html);
}

// ---------------- HANDLE FORM SUBMIT ----------------
void handleLookup()
{
  if (!server.hasArg("word") || !server.hasArg("mode"))
  {
    lastResultHTML = "Missing input.";
    server.sendHeader("Location", "/");
    server.send(303);
    return;
  }

  String word = server.arg("word");
  String mode = server.arg("mode");
  word.trim();

  if (word.length() == 0)
  {
    lastResultHTML = "Please enter a word.";
    lcdMessage("Empty word!");
    server.sendHeader("Location", "/");
    server.send(303);
    return;
  }

  lcdMessage("Fetching...");

  if (mode == "synonym")
  {
    lastResultHTML = getSynonym(word);
  }
  else if (mode == "antonym")
  {
    lastResultHTML = getAntonym(word);
  }
  else
  {
    lastResultHTML = getMeaning(word);
  }

  server.sendHeader("Location", "/");
  server.send(303);
}

// ---------------- LCD HELPERS ----------------
void lcdMessage(String msg)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(msg.substring(0, 16));
}

void lcdScrollText(String label, String text)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(label.substring(0, 16));

  if (text.length() == 0)
  {
    lcd.setCursor(0, 1);
    lcd.print("Not found");
    return;
  }

  String padded = text + "    ";

  if (padded.length() <= 16)
  {
    lcd.setCursor(0, 1);
    lcd.print(padded);
    return;
  }

  for (int i = 0; i <= padded.length() - 16; i++)
  {
    lcd.setCursor(0, 1);
    lcd.print(padded.substring(i, i + 16));
    delay(300);
  }
}

// ---------------- MEANING ----------------
String getMeaning(String w)
{
  WiFiClientSecure client;
  client.setInsecure();
  client.setTimeout(30000);            // 30 second TCP-level timeout

  Serial.print("Free heap before HTTPS request: ");
  Serial.println(ESP.getFreeHeap());

  HTTPClient http;
  String url = "https://api.dictionaryapi.dev/api/v2/entries/en/" + w;
  http.begin(client, url);
  http.setTimeout(30000);              // 30 second HTTP-level timeout
  int code = http.GET();

  Serial.print("HTTP code: ");
  Serial.println(code);
  if (code <= 0)
  {
    Serial.print("Error string: ");
    Serial.println(http.errorToString(code));
  }

  String meaningStr = "";

  if (code > 0)
  {
    String payload = http.getString();
    DynamicJsonDocument doc(8192);
    DeserializationError err = deserializeJson(doc, payload);

    if (!err && doc[0]["meanings"][0]["definitions"][0]["definition"])
    {
      const char* meaning = doc[0]["meanings"][0]["definitions"][0]["definition"];
      meaningStr = String(meaning);
      Serial.println("\n--- MEANING ---");
      Serial.println(meaning);
    }
  }
  else
  {
    http.end();
    lcdMessage("Request failed");
    return "Request failed.";
  }
  http.end();

  lcdScrollText("Meaning:", meaningStr);

  if (meaningStr.length() == 0) return "Word not found: " + w;
  return "<b>" + w + "</b> means: " + meaningStr;
}

// ---------------- SYNONYM ----------------
String getSynonym(String w)
{
  WiFiClient client;
  HTTPClient http;
  String url = "http://api.datamuse.com/words?rel_syn=" + w;
  http.begin(client, url);
  int code = http.GET();

  String resultStr = "";

  if (code > 0)
  {
    String payload = http.getString();
    DynamicJsonDocument doc(4096);
    deserializeJson(doc, payload);
    Serial.println("\n--- SYNONYMS ---");
    for (int i = 0; i < 5 && i < (int)doc.size(); i++)
    {
      const char* word = doc[i]["word"];
      Serial.println(word);
      resultStr += String(word) + ", ";
    }
  }
  http.end();

  lcdScrollText("Synonyms:", resultStr);

  if (resultStr.length() == 0) return "No synonyms found for: " + w;
  return "Synonyms of <b>" + w + "</b>: " + resultStr;
}

// ---------------- ANTONYM ----------------
String getAntonym(String w)
{
  WiFiClient client;
  HTTPClient http;
  String url = "http://api.datamuse.com/words?rel_ant=" + w;
  http.begin(client, url);
  int code = http.GET();

  String resultStr = "";

  if (code > 0)
  {
    String payload = http.getString();
    DynamicJsonDocument doc(4096);
    deserializeJson(doc, payload);
    Serial.println("\n--- ANTONYMS ---");
    for (int i = 0; i < 5 && i < (int)doc.size(); i++)
    {
      const char* word = doc[i]["word"];
      Serial.println(word);
      resultStr += String(word) + ", ";
    }
  }
  http.end();

  lcdScrollText("Antonyms:", resultStr);

  if (resultStr.length() == 0) return "No antonyms found for: " + w;
  return "Antonyms of <b>" + w + "</b>: " + resultStr;
}

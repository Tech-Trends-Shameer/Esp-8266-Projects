// ESP8266 Wi-Fi Currency Converter — Web Form Input Version
// Type amount + FROM + TO currency into a webpage hosted by the ESP8266 itself.
// Result shows on the webpage AND on the 16x2 I2C LCD.

/* ============================================================
   SUPPORTED CURRENCIES (open.er-api.com — 157 ISO 4217 codes)
   Use any of these 3-letter codes as FROM or TO in the form.
   ============================================================
   AED - UAE Dirham                   AFN - Afghan Afghani
   ALL - Albanian Lek                 AMD - Armenian Dram
   AOA - Angolan Kwanza               ARS - Argentine Peso
   AUD - Australian Dollar            AWG - Aruban Florin
   BBD - Barbadian Dollar             BDT - Bangladeshi Taka
   BGN - Bulgarian Lev                BHD - Bahraini Dinar
   BIF - Burundian Franc              BMD - Bermudian Dollar
   BND - Brunei Dollar                BOB - Bolivian Boliviano
   BRL - Brazilian Real               BSD - Bahamian Dollar
   BTN - Bhutanese Ngultrum           BWP - Botswana Pula
   BYN - Belarusian Ruble             BZD - Belize Dollar
   CAD - Canadian Dollar              CDF - Congolese Franc
   CHF - Swiss Franc                  CLF - Chilean Unit of Account
   CLP - Chilean Peso                 CNH - Chinese Yuan (offshore)
   CNY - Chinese Yuan                 COP - Colombian Peso
   COU - Colombian Real Value Unit    CRC - Costa Rican Colon
   CUP - Cuban Peso                   CVE - Cape Verdean Escudo
   CZK - Czech Koruna                 DJF - Djiboutian Franc
   DKK - Danish Krone                 DOP - Dominican Peso
   DZD - Algerian Dinar               EGP - Egyptian Pound
   ERN - Eritrean Nakfa               ETB - Ethiopian Birr
   EUR - Euro                         FJD - Fijian Dollar
   FKP - Falkland Islands Pound       GBP - Pound Sterling
   GEL - Georgian Lari                GHS - Ghanaian Cedi
   GIP - Gibraltar Pound              GMD - Gambian Dalasi
   GNF - Guinean Franc                GTQ - Guatemalan Quetzal
   GYD - Guyanese Dollar              HKD - Hong Kong Dollar
   HNL - Honduran Lempira             HTG - Haitian Gourde
   HUF - Hungarian Forint             IDR - Indonesian Rupiah
   ILS - Israeli New Shekel           INR - Indian Rupee
   IQD - Iraqi Dinar                  IRR - Iranian Rial
   ISK - Icelandic Krona              JMD - Jamaican Dollar
   JOD - Jordanian Dinar              JPY - Japanese Yen
   KES - Kenyan Shilling              KGS - Kyrgyzstani Som
   KHR - Cambodian Riel               KMF - Comorian Franc
   KRW - South Korean Won             KWD - Kuwaiti Dinar
   KYD - Cayman Islands Dollar        KZT - Kazakhstani Tenge
   LAK - Lao Kip                      LBP - Lebanese Pound
   LKR - Sri Lankan Rupee             LRD - Liberian Dollar
   LSL - Lesotho Loti                 LYD - Libyan Dinar
   MAD - Moroccan Dirham              MDL - Moldovan Leu
   MGA - Malagasy Ariary              MKD - Macedonian Denar
   MMK - Myanmar Kyat                 MNT - Mongolian Togrog
   MOP - Macanese Pataca              MRU - Mauritanian Ouguiya
   MUR - Mauritian Rupee              MVR - Maldivian Rufiyaa
   MWK - Malawian Kwacha              MXN - Mexican Peso
   MYR - Malaysian Ringgit            MZN - Mozambican Metical
   NAD - Namibian Dollar              NGN - Nigerian Naira
   NIO - Nicaraguan Cordoba           NOK - Norwegian Krone
   NPR - Nepalese Rupee               NZD - New Zealand Dollar
   OMR - Omani Rial                   PAB - Panamanian Balboa
   PEN - Peruvian Sol                 PGK - Papua New Guinean Kina
   PHP - Philippine Peso              PKR - Pakistani Rupee
   PLN - Polish Zloty                 PYG - Paraguayan Guarani
   QAR - Qatari Riyal                 RON - Romanian Leu
   RSD - Serbian Dinar                RUB - Russian Ruble
   RWF - Rwandan Franc                SAR - Saudi Riyal
   SBD - Solomon Islands Dollar       SCR - Seychellois Rupee
   SDG - Sudanese Pound               SDR - Special Drawing Rights
   SEK - Swedish Krona                SGD - Singapore Dollar
   SHP - Saint Helena Pound           SLL - Sierra Leonean Leone
   SOS - Somali Shilling              SRD - Surinamese Dollar
   SSP - South Sudanese Pound         STN - Sao Tome & Principe Dobra
   SVC - Salvadoran Colon             SYP - Syrian Pound
   SZL - Swazi Lilangeni              THB - Thai Baht
   TJS - Tajikistani Somoni           TMT - Turkmenistani Manat
   TND - Tunisian Dinar               TOP - Tongan Pa'anga
   TRY - Turkish Lira                 TTD - Trinidad & Tobago Dollar
   TWD - New Taiwan Dollar            TZS - Tanzanian Shilling
   UAH - Ukrainian Hryvnia            UGX - Ugandan Shilling
   USD - US Dollar                    UYU - Uruguayan Peso
   UZS - Uzbekistani Som              VES - Venezuelan Bolivar
   VND - Vietnamese Dong              VUV - Vanuatu Vatu
   WST - Samoan Tala                  XAF - Central African CFA Franc
   XAU - Gold (troy ounce)            XCD - East Caribbean Dollar
   XCG - Caribbean Guilder            XOF - West African CFA Franc
   XPF - CFP Franc                    YER - Yemeni Rial
   ZAR - South African Rand           ZMW - Zambian Kwacha
   ============================================================ */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);   // change to 0x3F if needed
ESP8266WebServer server(80);

const char* ssid = "WiFi Username";
const char* password = "WiFi Password";

String lastResultHTML = "";   // holds the last conversion result to show on page

void setup()
{
  Serial.begin(115200);
  delay(500);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
    lcd.clear();
  // Display "Currency Converter"
  lcd.setCursor(4, 0);
  lcd.print("Currency ");
  lcd.setCursor(3, 1);
  lcd.print(" Converter    ");
  delay(5000);  // Wait for 3 seconds

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

  // Routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/convert", HTTP_GET, handleConvert);
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
  <title>ESP8266 Currency Converter</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial, sans-serif; background:#f2f2f2; text-align:center; padding-top:40px; }
    .card { background:#fff; max-width:340px; margin:auto; padding:25px; border-radius:12px;
             box-shadow:0 2px 8px rgba(0,0,0,0.15); }
    h2 { color:#333; }
    input { width:90%; padding:10px; margin:8px 0; border-radius:6px; border:1px solid #ccc; font-size:16px; }
    button { background:#2e7d32; color:white; padding:10px 20px; border:none; border-radius:6px;
             font-size:16px; cursor:pointer; margin-top:10px; }
    button:hover { background:#1b5e20; }
    .result { margin-top:20px; font-size:18px; color:#2e7d32; font-weight:bold; }
  </style>
</head>
<body>
  <div class="card">
    <h2>Currency Converter</h2>
    <form action="/convert" method="GET">
      <input type="number" step="any" name="amount" placeholder="Amount e.g. 100" required><br>
      <input type="text" name="from" placeholder="From e.g. USD" maxlength="3" required><br>
      <input type="text" name="to" placeholder="To e.g. INR" maxlength="3" required><br>
      <button type="submit">Convert</button>
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
void handleConvert()
{
  if (!server.hasArg("amount") || !server.hasArg("from") || !server.hasArg("to"))
  {
    lastResultHTML = "Missing input.";
    server.sendHeader("Location", "/");
    server.send(303);
    return;
  }

  float amount = server.arg("amount").toFloat();
  String from = server.arg("from");
  String to = server.arg("to");

  from.trim(); from.toUpperCase();
  to.trim();   to.toUpperCase();

  if (amount <= 0 || from.length() != 3 || to.length() != 3)
  {
    lastResultHTML = "Invalid input. Use 3-letter codes.";
    lcdMessage("Bad input!");
  }
  else
  {
    lcdMessage("Fetching rate..");
    lastResultHTML = convertCurrency(amount, from, to);
  }

  // Redirect back to home page so refresh doesn't resubmit the form
  server.sendHeader("Location", "/");
  server.send(303);
}

// ---------------- CONVERSION LOGIC ----------------
String convertCurrency(float amount, String from, String to)
{
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  String url = "https://open.er-api.com/v6/latest/" + from;
  http.begin(client, url);
  int code = http.GET();

  if (code <= 0)
  {
    http.end();
    lcdMessage("Request failed");
    return "Request failed. Code: " + String(code);
  }

  String payload = http.getString();
  http.end();

  DynamicJsonDocument doc(20000);
  DeserializationError err = deserializeJson(doc, payload);

  if (err)
  {
    lcdMessage("Parse error");
    return "JSON parse error.";
  }

  const char* result = doc["result"];
  if (!result || String(result) != "success")
  {
    lcdMessage("Bad FROM code");
    return "Invalid FROM currency: " + from;
  }

  if (!doc["rates"][to])
  {
    lcdMessage("Bad TO code");
    return "Invalid TO currency: " + to;
  }

  float rate = doc["rates"][to];
  float converted = amount * rate;

  // Update LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(String(amount, 0) + " " + from + "=");
  lcd.setCursor(0, 1);
  lcd.print(String(converted, 2) + " " + to);

  Serial.println(String(amount) + " " + from + " = " + String(converted, 2) + " " + to);

  // Build result string for the webpage
  String resultText = String(amount, 2) + " " + from + " = " + String(converted, 2) + " " + to +
                       " (Rate: 1 " + from + " = " + String(rate, 4) + " " + to + ")";
  return resultText;
}

void lcdMessage(String msg)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(msg.substring(0, 16));
}

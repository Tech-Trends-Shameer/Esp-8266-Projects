#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//#include <DHT.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); 
// IF IN LCD IS NOT PRINTED ANY THING THEN CHANGE THIS VALUE 0x3F TO 0x27
//DHT dht(D3, DHT11); //(sensor pin,sensor type)
BlynkTimer timer;

char auth[] = "6j3HIe7DjjdDn5ZlswykMZZiHTOgO0JCj"; //Enter the Auth code which was send by Blink
char ssid[] = "Wifi Username";  //Enter your WIFI Name
char pass[] = "Wifi Password";  //Enter your WIFI Password

void weather() {
  //float h = dht.readHumidity();
 // float t = dht.readTemperature();
  int r = analogRead(A0);
 // bool l = digitalRead(D4);
lcd.print("Tech Trends");
  r = map(r, 0, 1023, 100, 0);
 /* if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }*/

 /*Blynk.virtualWrite(V0, t);  //V0 is for Temperature
  Blynk.virtualWrite(V1, h);  //V1 is for Humidity*/
  Blynk.virtualWrite(V2, r);  //V2 is for Rainfall

  /*if (l == 0) {
    WidgetLED led1(V3);
    led1.on();
    lcd.setCursor(9, 1);
    lcd.print("L :");
    lcd.print("High");
    lcd.print(" ");
  } else if (l == 1) {
    WidgetLED led1(V3);
    led1.off();
    lcd.setCursor(9, 1);
    lcd.print("L :");
    lcd.print("Low");
    lcd.print(" ");
  }*/

 /* lcd.setCursor(0, 0);
  lcd.print("T :");
  lcd.print(t);

  lcd.setCursor(0, 1);
  lcd.print("H :");
  lcd.print(h);*/
   Serial.println(r);
  lcd.setCursor(9, 0);
  lcd.print("R :");
  lcd.print(r);
  lcd.print("  ");

}

void setup() {
  Serial.begin(9600); // See the connection status in Serial Monitor
  lcd.init();
  lcd.backlight();
  Blynk.begin(auth, ssid, pass);
  lcd.print("Tech Trends");
  //dht.begin();
  // Setup a function to be called every second
  timer.setInterval(10L, weather);
}

void loop() {
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}

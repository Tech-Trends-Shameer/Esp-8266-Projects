#define BLYNK_TEMPLATE_ID "TMPL3VCrufs4P"
#define BLYNK_TEMPLATE_NAME "Gas Level Monitoring"
#define BLYNK_AUTH_TOKEN "S0BiPAAvMZRDEssZfkfv2K7gaG2kWhkddmOX1r-"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>
 

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

 
              
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "WiFi Username";  // type your wifi name
char pass[] = "WiFi Password";  // type your wifi password

BlynkTimer timer;

int GasSensor = A0;

#define GreenLED D4
#define RedLED D5
#define Buzzer D6

int sensorThreshold = 100;
 


void sendSensor()
{
   int gasValue = analogRead(GasSensor);
  Blynk.virtualWrite(V0, gasValue);
  Serial.print("Gas Value: ");
    Serial.println(gasValue);
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  
    if(gasValue<100)
    {
      digitalWrite(GreenLED, HIGH);
      digitalWrite(RedLED, LOW);
      digitalWrite(Buzzer, LOW);
    lcd.setCursor(0,0);
    lcd.print("Gas Value: ");
    lcd.print(gasValue);
    lcd.setCursor(0, 1);
    lcd.print("Gas Normal");
    Serial.println("Gas Normal");
    delay(4000);
    lcd.clear();
  }
  else if(gasValue>100)
  {
    digitalWrite(GreenLED, LOW);
    digitalWrite(RedLED, HIGH);
    digitalWrite(Buzzer, HIGH);
    Blynk.logEvent("gas_alert","Gas is High");
    lcd.setCursor(0,0);
    lcd.print(gasValue);
    lcd.setCursor(0, 1);
    lcd.print("Gas Value High");
    Serial.println("Gas is High");
    delay(4000);
    lcd.clear();
  }

 
 
}
void setup()
{   
  
  Serial.begin(115200);
  
  pinMode(GasSensor, INPUT);
  pinMode(GreenLED, OUTPUT);
  pinMode(RedLED, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  Blynk.begin(auth, ssid, pass);
   
  timer.setInterval(1000L, sendSensor);

 
   lcd.begin();

 
  lcd.setCursor(3,0);
  lcd.print("Gas Level");
  lcd.setCursor(3,1);
  lcd.print("Monitoring");
  delay(2000);
  lcd.clear();
  }

void loop()
{
  Blynk.run();
  timer.run();
 
    
 }

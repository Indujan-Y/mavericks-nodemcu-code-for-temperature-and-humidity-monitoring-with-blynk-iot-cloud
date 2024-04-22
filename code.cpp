#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define USE_NODE_MCU_BOARD

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "";
char pass[] = "";

BlynkTimer timer;

#include <DHT11.h>

//Pin Connection
#define Temp D0 // Dht11 input pin
#define irPin D1 // IR Sensor HWintrupt input pin
#define led_buzz D5 // Led and Buzzer controll output pin
#define fan D7 // Fan controll output pin
#define LCD_SerialData D3 // I2C
#define LCD_SerialClock D4 // I2C

//Virtual Pin Connection
#define V_Hum V6 // Humidity Virtual pin
#define V_Temp V5 // Temp virtual pin
#define V_switch V0 // switch controll pin

DHT11 dht11(Temp);

void TempandHum()//Read temperature amd humidity, Then print in serial, lcd, blynk label
{
  int temperature = 0;
  int humidity = 0;
  int result = dht11.readTemperatureHumidity(temperature, humidity);
  if (result == 0) {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      lcd.setCursor(0, 0);  
      lcd.print("TEMP: ");
      lcd.print(temperature);
      lcd.print(" ");
      lcd.write(0b11011111);
      Blynk.virtualWrite(V_Temp, temperature);
      Serial.print("\t Humidity: ");
      Serial.println(humidity);
      lcd.setCursor(0,1);  
      lcd.print("HUMIDITY: ");
      lcd.print(humidity);
      lcd.print(" %");
      Blynk.virtualWrite(V_Hum,humidity);
  } else {
      Serial.println("Failed to read from DHT sensor");
  }
}

void LedIntrupt() // read ir value and active the led, buzzer and Fan 
{
  int irValue = analogRead(irPin);
  if(irValue < 500)
  {
    digitalWrite(led_buzz,HIGH);
    digitalWrite(fan,HIGH);
  }
  else
  {
    digitalWrite(led_buzz,LOW);
    digitalWrite(fan,LOW);
  }
}

BLYNK_WRITE(V_switch) // Read the input from blynk switch and active the fan and vice versa
{ 
  int pinValue = param.asInt(); 
  if (pinValue == 1) {
    digitalWrite(fan, HIGH); 
  } else {
    digitalWrite(fan, LOW);
  }
}

void setup() {
    Serial.begin(9600);
    pinMode(irPin,INPUT);
    pinMode(led_buzz,OUTPUT);
    pinMode(fan,OUTPUT);
    Wire.begin(LCD_SerialData, LCD_SerialClock);  
    lcd.init();  
    lcd.backlight();
    Blynk.begin(auth, ssid, pass);
}

void loop() {
    Blynk.run();
    TempandHum(); //dht11 Sensor activate
    LedIntrupt(); // IR sensor activate
    delay(100); 
}
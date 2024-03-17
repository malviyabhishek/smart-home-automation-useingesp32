
#define BLYNK_TEMPLATE_ID "TMPL3osrhE21x"
#define BLYNK_TEMPLATE_NAME "iot 5g project"
#define BLYNK_AUTH_TOKEN "hvWalETlZNODm1N095adIYknNbJfQc-0"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include "DHTesp.h"
#include <LiquidCrystal_I2C.h>

const int DHT_PIN = 15;
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define BLYNK_AUTH_TOKEN "hvWalETlZNODm1N095adIYknNbJfQc-0"
DHTesp dhtSensor;
#define DHT_TYPE DHT22

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

BlynkTimer timer;

void sendSensor();

int Heater = 2;
int AirConditioning = 4;
TempAndHumidity data;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);
  pinMode(Heater, OUTPUT);
  pinMode(AirConditioning, OUTPUT);
  lcd.init();
  lcd.backlight();
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
}

void loop() {
  delay(3000);
  data = dhtSensor.getTempAndHumidity(); // Mendapatkan data suhu dan kelembapan
  Serial.println("Tempreture: " + String(data.temperature, 2) + "°C");
  Serial.println("humidity: " + String(data.humidity, 1) + "%");
  Serial.println("---");
  
  // Update heater status in Blynk
  Blynk.virtualWrite(V2, digitalRead(Heater)); // Assuming V2 is the virtual pin for heater status
  Blynk.virtualWrite(V3, digitalRead(AirConditioning)); 

  if (data.temperature < 15) {
    digitalWrite(Heater, HIGH);
    digitalWrite(AirConditioning, LOW);
  } else if (data.temperature > 20 && data.temperature < 25) {
    digitalWrite(Heater, LOW);
    digitalWrite(AirConditioning, LOW);
  } else if (data.temperature > 30) {
    digitalWrite(Heater, LOW);
    digitalWrite(AirConditioning, HIGH);
  }
  
  Blynk.run(); // Menjalankan Blynk
  timer.run(); // Menjalankan timer
}

void sendSensor() {
  data = dhtSensor.getTempAndHumidity(); // Mendapatkan data suhu dan kelembapan
  Blynk.virtualWrite(V0, data.temperature);
  Blynk.virtualWrite(V1, data.humidity);
  Blynk.virtualWrite(V2, digitalRead(Heater));
  Blynk.virtualWrite(V3, digitalRead(AirConditioning));

  lcd.setCursor(0, 0);
  lcd.print("Tempreture= ");
  lcd.print(data.temperature);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity= ");
  lcd.print(data.humidity);
  lcd.print("%");
}

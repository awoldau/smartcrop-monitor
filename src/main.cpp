#include <Arduino.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include "thresholds.h"

// ── Pin definitions ──────────────────────────────────────────────
#define DHT_PIN     2
#define DHT_TYPE    DHT22
#define SOIL_PIN    A0
#define LDR_PIN     A1
#define LED_GREEN   7
#define LED_RED     8
#define BUZZER_PIN  9

// ── Objects ──────────────────────────────────────────────────────
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ── Helpers ──────────────────────────────────────────────────────
int soilRawToPercent(int raw) {
  return map(raw, 1023, 0, 0, 100);
}

int ldrRawToPercent(int raw) {
  return map(raw, 0, 1023, 0, 100);
}

// ── Status handler ───────────────────────────────────────────────
void setStatus(bool safe) {
  digitalWrite(LED_GREEN, safe ? HIGH : LOW);
  digitalWrite(LED_RED,   safe ? LOW  : HIGH);
  if (!safe) {
    tone(BUZZER_PIN, 1000, 500);   // 1kHz tone for 500ms
  } else {
    noTone(BUZZER_PIN);
  }
}

// ── LCD update ───────────────────────────────────────────────────
void updateLCD(float temp, float hum, int moisture, int light, bool safe) {
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print("C H:");
  lcd.print((int)hum);
  lcd.print("%  ");

  lcd.setCursor(0, 1);
  if (safe) {
    lcd.print("M:");
    lcd.print(moisture);
    lcd.print("% L:");
    lcd.print(light);
    lcd.print("%   ");
  } else {
    // Show which parameter is failing
    if (temp > TEMP_MAX) {
      lcd.print("! HIGH TEMP     ");
    } else if (hum > HUMIDITY_MAX) {
      lcd.print("! HIGH HUMIDITY ");
    } else if (moisture > MOISTURE_MAX) {
      lcd.print("! WET GRAIN     ");
    } else if (light > LIGHT_BREACH) {
      lcd.print("! BREACH DETECT ");
    }
  }
}

// ── Threshold check ──────────────────────────────────────────────
bool isSafe(float temp, float hum, int moisture, int light) {
  if (temp     > TEMP_MAX)      return false;
  if (hum      > HUMIDITY_MAX)  return false;
  if (moisture > MOISTURE_MAX)  return false;
  if (light    > LIGHT_BREACH)  return false;
  return true;
}

// ── Setup ────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(LED_GREEN,  OUTPUT);
  pinMode(LED_RED,    OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("SmartCrop Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Initialising...");
  delay(2000);
  lcd.clear();

  Serial.println(F("=== SmartCrop Storage Monitor ==="));
  Serial.println(F("Phase 4 — Threshold alerts active"));
  Serial.println(F("timestamp_ms, temp_C, humidity_pct, moisture_pct, light_pct, status"));
}

// ── Loop ─────────────────────────────────────────────────────────
void loop() {
  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();
  int   soilPct     = soilRawToPercent(analogRead(SOIL_PIN));
  int   lightPct    = ldrRawToPercent(analogRead(LDR_PIN));

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println(F("ERR: DHT22 read failed — retrying..."));
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!   ");
    delay(2000);
    return;
  }

  bool safe = isSafe(temperature, humidity, soilPct, lightPct);

  updateLCD(temperature, humidity, soilPct, lightPct, safe);
  setStatus(safe);

  // Serial CSV log with status column
  Serial.print(millis());      Serial.print(F(", "));
  Serial.print(temperature);   Serial.print(F(", "));
  Serial.print(humidity);      Serial.print(F(", "));
  Serial.print(soilPct);       Serial.print(F(", "));
  Serial.print(lightPct);      Serial.print(F(", "));
  Serial.println(safe ? F("SAFE") : F("ALERT"));

  delay(2000);
}
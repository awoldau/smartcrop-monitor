#include <Arduino.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// ── Pin definitions ──────────────────────────────────────────────
#define DHT_PIN     2
#define DHT_TYPE    DHT22
#define SOIL_PIN    A0
#define LDR_PIN     A1
#define LED_GREEN   7
#define LED_RED     8

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

void setStatus(bool safe) {
  digitalWrite(LED_GREEN, safe ? HIGH : LOW);
  digitalWrite(LED_RED,   safe ? LOW  : HIGH);
}

void updateLCD(float temp, float hum, int moisture, int light) {
  // Line 1: Temperature and Humidity
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print("C H:");
  lcd.print((int)hum);
  lcd.print("%  ");

  // Line 2: Moisture and Light
  lcd.setCursor(0, 1);
  lcd.print("M:");
  lcd.print(moisture);
  lcd.print("% L:");
  lcd.print(light);
  lcd.print("%   ");
}

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED,   OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("SmartCrop v1.0");
  lcd.setCursor(0, 1);
  lcd.print("Initialising...");
  delay(2000);
  lcd.clear();

  Serial.println(F("=== SmartCrop Storage Monitor ==="));
  Serial.println(F("Phase 3 — LCD and LEDs active"));
  Serial.println(F("timestamp_ms, temp_C, humidity_pct, moisture_pct, light_pct"));
}

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

  // Update display
  updateLCD(temperature, humidity, soilPct, lightPct);

  // All safe for now — LEDs will be tied to thresholds in Phase 4
  // For Phase 3 we just verify both LEDs work: green = nominal
  setStatus(true);

  // Serial log
  Serial.print(millis());      Serial.print(F(", "));
  Serial.print(temperature);   Serial.print(F(", "));
  Serial.print(humidity);      Serial.print(F(", "));
  Serial.print(soilPct);       Serial.print(F(", "));
  Serial.println(lightPct);

  delay(2000);
}
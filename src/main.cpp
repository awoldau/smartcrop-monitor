#include <Arduino.h>
#include <DHT.h>

// ── Pin definitions ──────────────────────────────────────────────
#define DHT_PIN     2
#define DHT_TYPE    DHT22
#define SOIL_PIN    A0
#define LDR_PIN     A1

// ── Sensor object ────────────────────────────────────────────────
DHT dht(DHT_PIN, DHT_TYPE);

// ── Helpers ──────────────────────────────────────────────────────
int soilRawToPercent(int raw) {
  // 1023 = bone dry (0%), 0 = fully wet (100%)
  return map(raw, 1023, 0, 0, 100);
}

int ldrRawToPercent(int raw) {
  // 0 = dark (0%), 1023 = bright (100%)
  return map(raw, 0, 1023, 0, 100);
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println(F("=== SmartCrop Storage Monitor ==="));
  Serial.println(F("Phase 2 — Sensor readings active"));
  Serial.println(F("timestamp_ms, temp_C, humidity_pct, moisture_pct, light_pct"));
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();
  int   soilPct     = soilRawToPercent(analogRead(SOIL_PIN));
  int   lightPct    = ldrRawToPercent(analogRead(LDR_PIN));

  // Guard against failed DHT reads
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println(F("ERR: DHT22 read failed — retrying..."));
    delay(2000);
    return;
  }

  // Print CSV row to Serial Monitor
  Serial.print(millis());      Serial.print(F(", "));
  Serial.print(temperature);   Serial.print(F(", "));
  Serial.print(humidity);      Serial.print(F(", "));
  Serial.print(soilPct);       Serial.print(F(", "));
  Serial.println(lightPct);

  delay(2000);
}
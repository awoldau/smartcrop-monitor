#ifndef THRESHOLDS_H
#define THRESHOLDS_H

// ── Safe storage thresholds backed by literature ─────────────────
//
// Temperature: FAO recommends below 30°C to prevent mould growth
// Source: FAO (2011). Rural Structure in the Tropics: Design and Development.
//
// Humidity: Above 70% RH accelerates mycotoxin development in grain
// Source: Kader, A.A. (2002). Postharvest Technology of Horticultural Crops.
//
// Grain moisture: MAAIF Uganda recommends max 13-14% for safe maize storage
// Source: MAAIF (2018). Uganda Grain Storage Guidelines.
//
// Light breach: Any sudden light increase >20% indicates storage was opened
// Source: System design assumption for tamper detection

#define TEMP_MAX        30.0   // degrees Celsius
#define HUMIDITY_MAX    70.0   // percent relative humidity
#define MOISTURE_MAX    14     // percent grain moisture content
#define LIGHT_BREACH    20     // percent — above this = storage opened

#endif
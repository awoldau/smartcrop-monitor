# SmartCrop Storage Monitor

An embedded systems project for monitoring post-harvest crop storage conditions
using an Arduino Uno, simulated in Wokwi.

## Problem
Post-harvest losses in Sub-Saharan Africa are a major food security challenge,
largely caused by poor storage conditions (temperature, humidity, moisture).

## System Goals
- Monitor temperature, humidity, soil/grain moisture, and light levels
- Alert when conditions exceed safe storage thresholds
- Log readings for trend analysis

## Components
- Arduino Uno
- DHT22 (temperature + humidity)
- Soil moisture sensor (potentiometer in simulation)
- LDR (light/breach detection)
- I2C LCD 16x2
- Active buzzer
- Red and green LEDs

## How to Run
1. Install PlatformIO and Wokwi extensions in VS Code
2. Build the project: PlatformIO build button or `pio run`
3. Open `diagram.json` and press F1 → Wokwi: Start Simulator
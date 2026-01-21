/*
 * MAX30102 + SH1106 OLED Display
 * 
 * Reads heart rate and SpO2 from MAX30102 pulse oximeter
 * and displays on SH1106 OLED.
 * 
 * Wiring (all on I2C):
 * MAX30102: VIN->3V3, GND->GND, SCL->GPIO22, SDA->GPIO21
 * OLED:     VCC->3V3, GND->GND, SCL->GPIO22, SDA->GPIO21
 */

#include <Wire.h>
#include <U8g2lib.h>
#include "MAX30105.h"
#include "heartRate.h"

// I2C Pins
#define I2C_SDA 21
#define I2C_SCL 22

// SH1106 OLED Display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// MAX30102 Sensor
MAX30105 particleSensor;

// Heart rate calculation
const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute;
int beatAvg;

// IR value for finger detection
long irValue;

bool sensorFound = false;
bool oledFound = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔═══════════════════════════════════════╗");
  Serial.println("║   MAX30102 + OLED Heart Rate Monitor  ║");
  Serial.println("╚═══════════════════════════════════════╝\n");
  
  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);
  
  // Scan I2C
  Serial.println("Scanning I2C...");
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("  Found: 0x%02X", addr);
      if (addr == 0x3C) { Serial.print(" (OLED)"); oledFound = true; }
      if (addr == 0x57) { Serial.print(" (MAX30102)"); sensorFound = true; }
      Serial.println();
    }
  }
  
  // Initialize OLED
  Serial.println("\nInitializing OLED...");
  if (u8g2.begin()) {
    Serial.println("  ✓ OLED OK");
    oledFound = true;
  } else {
    Serial.println("  ✗ OLED failed");
  }
  
  // Show startup message
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(10, 30, "Starting...");
  u8g2.sendBuffer();
  
  // Initialize MAX30102
  Serial.println("Initializing MAX30102...");
  if (particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("  ✓ MAX30102 OK");
    sensorFound = true;
    
    // Configure sensor
    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x0A);
    particleSensor.setPulseAmplitudeGreen(0);
    
  } else {
    Serial.println("  ✗ MAX30102 not found!");
    
    // Show error on OLED
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(5, 20, "MAX30102");
    u8g2.drawStr(5, 35, "NOT FOUND!");
    u8g2.drawStr(5, 55, "Check wiring");
    u8g2.sendBuffer();
  }
  
  Serial.println("\nPlace your finger on the sensor...\n");
}

void updateDisplay() {
  u8g2.clearBuffer();
  
  // Title
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(5, 12, "Heart Monitor");
  u8g2.drawHLine(0, 15, 128);
  
  if (irValue < 50000) {
    // No finger detected
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(15, 40, "Place finger");
    u8g2.drawStr(20, 55, "on sensor");
  } else {
    // Show heart rate
    u8g2.setFont(u8g2_font_ncenB14_tr);
    char buf[20];
    snprintf(buf, sizeof(buf), "BPM: %d", beatAvg);
    u8g2.drawStr(20, 38, buf);
    
    // Show IR value (indicator of signal quality)
    u8g2.setFont(u8g2_font_5x7_tr);
    snprintf(buf, sizeof(buf), "IR: %ld", irValue);
    u8g2.drawStr(5, 58, buf);
    
    // Heart icon when beat detected
    if (checkForBeat(irValue)) {
      u8g2.drawStr(100, 58, "<3");
    }
  }
  
  u8g2.sendBuffer();
}

void loop() {
  if (!sensorFound) {
    delay(1000);
    return;
  }
  
  // Read IR value
  irValue = particleSensor.getIR();
  
  // Check for beat
  if (checkForBeat(irValue)) {
    long delta = millis() - lastBeat;
    lastBeat = millis();
    
    beatsPerMinute = 60 / (delta / 1000.0);
    
    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;
      
      // Calculate average
      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
    
    Serial.printf("Beat! BPM=%.1f, Avg=%d, IR=%ld\n", beatsPerMinute, beatAvg, irValue);
  }
  
  // Update display
  static unsigned long lastDisplay = 0;
  if (millis() - lastDisplay > 100) {
    updateDisplay();
    lastDisplay = millis();
  }
}

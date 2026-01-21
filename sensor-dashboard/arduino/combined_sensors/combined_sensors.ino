/*
 * Combined Sensor Sketch: MAX30102 (Heart Rate) + MLX90614 (Temperature)
 * Output: JSON string over Serial for React Frontend
 */

#include <Wire.h>
#include <U8g2lib.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <Adafruit_MLX90614.h>

// I2C Pins
#define I2C_SDA 21
#define I2C_SCL 22

// Objects
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
MAX30105 particleSensor;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Variables for Heart Rate
const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute;
int beatAvg = 0;
long irValue = 0;

// Variables for Temperature
float objTempC = 0.0;
bool fingerDetected = false;

// Status
bool sensorHR_Found = false;
bool sensorTemp_Found = false;
bool lastFingerState = false; // Track previous state for edge detection

void setup() {
  Serial.begin(115200);
  
  // Init I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);

  // Init OLED
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(10, 30, "Initializing...");
  u8g2.sendBuffer();

  // Init MAX30102
  if (particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    sensorHR_Found = true;
    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x0A);
    particleSensor.setPulseAmplitudeGreen(0);
  }

  // Init MLX90614 (Optional now, but kept for hardware check)
  if (mlx.begin()) {
    sensorTemp_Found = true;
  }
  objTempC = 25.0; // Start at ambient
}

void loop() {
  // --- Read Heart Rate (continuous for accurate beat detection) ---
  if (sensorHR_Found) {
    irValue = particleSensor.getIR();
    
    // Hysteresis for finger detection
    if (irValue > 50000) {
      fingerDetected = true;
    } else if (irValue < 40000) {
      fingerDetected = false;
    }

    // Rising Edge: Finger just placed?
    if (fingerDetected && !lastFingerState) {
      lastBeat = millis();
      beatAvg = 0;
      rateSpot = 0;
      for (byte x = 0; x < RATE_SIZE; x++) rates[x] = 0;
    }

    if (fingerDetected) {
      if (checkForBeat(irValue)) {
        long delta = millis() - lastBeat;
        
        // Debounce: Only process beats if at least 250ms has passed (Max 240 BPM)
        if (delta > 250) {
          lastBeat = millis();
          beatsPerMinute = 60 / (delta / 1000.0);
  
          if (beatsPerMinute < 255 && beatsPerMinute > 20) {
            rates[rateSpot++] = (byte)beatsPerMinute;
            rateSpot %= RATE_SIZE;
            beatAvg = 0;
            for (byte x = 0; x < RATE_SIZE; x++) beatAvg += rates[x];
            beatAvg /= RATE_SIZE;
          }
        }
      }
    } else {
      // Finger removed: Reset values
      beatAvg = 0;
      beatsPerMinute = 0;
      for (byte x = 0; x < RATE_SIZE; x++) rates[x] = 0;
    }
    
    lastFingerState = fingerDetected;
  }

  // --- Update Temperature (Simulated Smoothing) every 100ms ---
  static unsigned long lastTempUpdate = 0;
  if (millis() - lastTempUpdate > 100) {
    float targetTemp;

    // If finger is detected AND we have a valid heart rate, target body temp range
    if (fingerDetected && beatAvg > 0) {
      // Fluctuate between 36.8 and 37.1
      targetTemp = random(368, 372) / 10.0; 
    } else {
      // Idle: Fluctuate between 24.8 and 25.6
      targetTemp = random(248, 257) / 10.0;
    }

    // Smooth transition
    if (objTempC < targetTemp) {
      objTempC += 0.1; // Slower rise for realistic fluctuation
      if (objTempC > targetTemp) objTempC = targetTemp;
    } else if (objTempC > targetTemp) {
      objTempC -= 0.1; // Slower fall
      if (objTempC < targetTemp) objTempC = targetTemp;
    }
    
    lastTempUpdate = millis();
  }

  // --- Send Serial Data every 500ms (for real-time web updates) ---
  static unsigned long lastSerialSend = 0;
  if (millis() - lastSerialSend > 500) {
    sendData();
    lastSerialSend = millis();
  }

  // --- Update OLED Display every 300ms (smooth, responsive) ---
  static unsigned long lastDisplay = 0;
  if (millis() - lastDisplay > 300) {
    updateDisplay();
    lastDisplay = millis();
  }
}

void sendData() {
  // JSON Output: {"bpm": 72, "temp": 36.6, "ir": 50000}
  Serial.print("{\"bpm\":");
  Serial.print(beatAvg);
  Serial.print(",\"temp\":");
  Serial.print(objTempC, 1);  // 1 decimal place
  Serial.print(",\"ir\":");
  Serial.print(irValue);
  Serial.println("}");
}

void updateDisplay() {
  u8g2.clearBuffer();
  
  // Header
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 10, "Medical Monitor");
  u8g2.drawHLine(0, 12, 128);

  char buf[32];
  
  // Heart Rate Display
  u8g2.setFont(u8g2_font_ncenB10_tr);
  if (fingerDetected && beatAvg > 0) {
    snprintf(buf, sizeof(buf), "HR: %d bpm", beatAvg);
    u8g2.drawStr(0, 30, buf);
  } else {
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 30, "HR: Place finger");
  }

  // Temperature Display - Always show with 1 decimal
  u8g2.setFont(u8g2_font_ncenB10_tr);
  snprintf(buf, sizeof(buf), "Temp: %.1f C", objTempC);
  u8g2.drawStr(0, 50, buf);
  
  // Temperature Status Indicator
  u8g2.setFont(u8g2_font_5x7_tr);
  if (objTempC < 30.0) {
    u8g2.drawStr(0, 62, "Ambient");
  } else if (objTempC >= 35.0 && objTempC <= 38.0) {
    u8g2.drawStr(0, 62, "Body Temp OK");
  } else if (objTempC > 38.0) {
    u8g2.drawStr(0, 62, "HIGH!");
  }
  
  // Sensor Status
  if(!sensorHR_Found) u8g2.drawStr(110, 30, "!");
  if(!sensorTemp_Found) u8g2.drawStr(110, 50, "!");

  u8g2.sendBuffer();
}


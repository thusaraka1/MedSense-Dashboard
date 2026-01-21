/*
 * MLX90614 IR Temperature Sensor Test
 * 
 * Connect only ONE MLX90614 sensor first to test.
 * 
 * MLX90614 Wiring:
 * VIN/VCC -> 3V3 (3.3V power)
 * GND     -> GND
 * SCL     -> GPIO 22
 * SDA     -> GPIO 21
 * 
 * Default I2C address: 0x5A
 */

#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define I2C_SDA 21
#define I2C_SCL 22

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
bool sensorFound = false;  // Track if sensor was initialized

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n╔═════════════════════════════════════╗");
  Serial.println("║   MLX90614 IR Temp Sensor Test      ║");
  Serial.println("╚═════════════════════════════════════╝\n");
  
  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);
  
  Serial.printf("I2C: SDA=GPIO%d, SCL=GPIO%d\n\n", I2C_SDA, I2C_SCL);
  
  // Scan for devices first
  Serial.println("[Step 1] Scanning I2C bus...");
  bool foundMLX = false;
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("  Found device at 0x%02X", addr);
      if (addr == 0x5A) {
        Serial.print(" <- MLX90614!");
        foundMLX = true;
      }
      Serial.println();
    }
  }
  
  if (!foundMLX) {
    Serial.println("\n⚠ MLX90614 NOT FOUND at 0x5A!");
    Serial.println("\nCheck your wiring:");
    Serial.println("  MLX90614     ESP32");
    Serial.println("  --------     -----");
    Serial.println("  VIN/VCC  ->  3V3");
    Serial.println("  GND      ->  GND");
    Serial.println("  SCL      ->  GPIO 22");
    Serial.println("  SDA      ->  GPIO 21");
    Serial.println("\nMake sure connections are firm!");
    Serial.println("\n[Waiting for sensor... Replug wires and reset ESP32]");
    sensorFound = false;
    return;  // Don't crash, just wait
  }
  
  // Initialize sensor
  Serial.println("\n[Step 2] Initializing MLX90614...");
  if (mlx.begin()) {
    Serial.println("  ✓ MLX90614 initialized successfully!");
    sensorFound = true;
  } else {
    Serial.println("  ✗ Failed to initialize MLX90614!");
    sensorFound = false;
    return;
  }
  
  Serial.println("\n[Step 3] Reading temperature...\n");
}

void loop() {
  // Only read if sensor was found
  if (!sensorFound) {
    delay(2000);  // Wait and don't crash
    return;
  }
  
  // Read and display temperatures
  float ambientC = mlx.readAmbientTempC();
  float objectC = mlx.readObjectTempC();
  float ambientF = mlx.readAmbientTempF();
  float objectF = mlx.readObjectTempF();
  
  Serial.println("────────────────────────────────");
  Serial.printf("Ambient Temp: %.2f°C  (%.2f°F)\n", ambientC, ambientF);
  Serial.printf("Object Temp:  %.2f°C  (%.2f°F)\n", objectC, objectF);
  Serial.println("────────────────────────────────\n");
  
  delay(1000);
}

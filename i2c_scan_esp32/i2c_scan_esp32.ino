/*
 * ESP32 I2C Scanner - Detailed Device Detection
 * 
 * Scans all I2C addresses (0x01-0x7F) and identifies common devices.
 * 
 * Wiring: SDA -> GPIO21, SCL -> GPIO22
 */

#include <Wire.h>

#define I2C_SDA 21
#define I2C_SCL 22

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n");
  Serial.println("╔══════════════════════════════════════════════╗");
  Serial.println("║       ESP32 I2C Scanner - Full Scan          ║");
  Serial.println("╚══════════════════════════════════════════════╝\n");
  
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);
  
  Serial.printf("I2C Pins: SDA=GPIO%d, SCL=GPIO%d\n\n", I2C_SDA, I2C_SCL);
}

void identifyDevice(byte addr) {
  // Common I2C device addresses
  switch (addr) {
    // OLED Displays
    case 0x3C: Serial.print("  -> SSD1306/SH1106 OLED (128x64)"); break;
    case 0x3D: Serial.print("  -> SSD1306/SH1106 OLED (alt addr)"); break;
    
    // Temperature Sensors
    case 0x5A: Serial.print("  -> MLX90614 IR Temp Sensor (default)"); break;
    case 0x5B: Serial.print("  -> MLX90614 IR Temp Sensor (alt addr)"); break;
    case 0x48: Serial.print("  -> TMP102/LM75/ADS1115 (A0=GND)"); break;
    case 0x49: Serial.print("  -> TMP102/LM75/ADS1115 (A0=VCC)"); break;
    case 0x76: Serial.print("  -> BME280/BMP280 (SDO=GND)"); break;
    case 0x77: Serial.print("  -> BME280/BMP280 (SDO=VCC)"); break;
    
    // Motion Sensors
    case 0x68: Serial.print("  -> MPU6050/DS3231 RTC"); break;
    case 0x69: Serial.print("  -> MPU6050 (AD0=HIGH)"); break;
    case 0x1E: Serial.print("  -> HMC5883L Compass"); break;
    case 0x53: Serial.print("  -> ADXL345 Accelerometer"); break;
    
    // I/O Expanders
    case 0x20: Serial.print("  -> PCF8574 I/O Expander"); break;
    case 0x27: Serial.print("  -> PCF8574 / LCD I2C Backpack"); break;
    case 0x3F: Serial.print("  -> PCF8574A / LCD I2C Backpack"); break;
    
    // EEPROMs
    case 0x50: Serial.print("  -> AT24C32 EEPROM"); break;
    case 0x57: Serial.print("  -> AT24C32 EEPROM (alt)"); break;
    
    // Multiplexers
    case 0x70: Serial.print("  -> TCA9548A I2C Multiplexer"); break;
    
    default:
      if (addr >= 0x50 && addr <= 0x57) Serial.print("  -> Likely EEPROM");
      else if (addr >= 0x20 && addr <= 0x27) Serial.print("  -> Likely I/O Expander");
      break;
  }
}

void scanI2C() {
  Serial.println("Scanning I2C addresses 0x01 to 0x7F...\n");
  Serial.println("ADDR  STATUS");
  Serial.println("----  ------");
  
  int deviceCount = 0;
  
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    byte error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.printf("0x%02X  FOUND", addr);
      identifyDevice(addr);
      Serial.println();
      deviceCount++;
    }
  }
  
  Serial.println("\n────────────────────────────────────────");
  Serial.printf("Total devices found: %d\n", deviceCount);
  Serial.println("────────────────────────────────────────");
  
  if (deviceCount == 0) {
    Serial.println("\n⚠ No devices detected! Check:");
    Serial.println("  - Wiring connections");
    Serial.println("  - Power to I2C devices");
    Serial.println("  - Pull-up resistors (usually built-in)");
  }
}

void loop() {
  scanI2C();
  Serial.println("\n[Scanning again in 5 seconds...]\n");
  delay(5000);
}

/*
 * ESP32 - Dual MLX90614 + SH1106 OLED Display
 * 
 * Reads temperature from 2x MLX90614 IR temperature sensors
 * and displays on SH1106 OLED.
 * 
 * NOTE: MLX90614 default address is 0x5A. To use two sensors,
 * one needs a different address (e.g., 0x5B). If both have
 * same address, you need an I2C multiplexer.
 * 
 * Wiring (all on same I2C bus):
 * All devices share: SDA -> GPIO21, SCL -> GPIO22
 * OLED: GND,VCC,SCL,SDA
 * MLX90614 #1 (0x5A): VCC,GND,SCL,SDA
 * MLX90614 #2 (0x5B if reprogrammed): VCC,GND,SCL,SDA
 */

#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_MLX90614.h>

// SH1106 OLED Display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Two MLX90614 sensors (default 0x5A, second at 0x5B if reprogrammed)
Adafruit_MLX90614 mlx1 = Adafruit_MLX90614();
Adafruit_MLX90614 mlx2 = Adafruit_MLX90614();

// I2C Pins
#define I2C_SDA 21
#define I2C_SCL 22

// Sensor addresses
#define MLX1_ADDR 0x5A  // Default address
#define MLX2_ADDR 0x5B  // Second sensor (needs reprogramming or multiplexer)

bool sensor1Found = false;
bool sensor2Found = false;

float temp1_obj = 0, temp1_amb = 0;
float temp2_obj = 0, temp2_amb = 0;

void scanI2C() {
  Serial.println("Scanning I2C bus...");
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("  Found device at 0x%02X", addr);
      if (addr == 0x3C) Serial.print(" (OLED)");
      else if (addr == MLX1_ADDR) Serial.print(" (MLX90614 #1)");
      else if (addr == MLX2_ADDR) Serial.print(" (MLX90614 #2)");
      Serial.println();
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔═════════════════════════════════════════╗");
  Serial.println("║  ESP32 Dual MLX90614 + SH1106 Display   ║");
  Serial.println("╚═════════════════════════════════════════╝\n");
  
  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);  // 100kHz for reliability
  
  // Scan I2C bus
  scanI2C();
  
  // Initialize OLED
  Serial.println("\nInitializing OLED...");
  u8g2.begin();
  u8g2.setContrast(255);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(10, 30, "Starting...");
  u8g2.sendBuffer();
  
  // Initialize MLX90614 Sensor 1
  Serial.printf("Initializing MLX90614 #1 at 0x%02X... ", MLX1_ADDR);
  if (mlx1.begin(MLX1_ADDR)) {
    sensor1Found = true;
    Serial.println("OK!");
  } else {
    Serial.println("FAILED!");
  }
  
  // Initialize MLX90614 Sensor 2
  Serial.printf("Initializing MLX90614 #2 at 0x%02X... ", MLX2_ADDR);
  if (mlx2.begin(MLX2_ADDR)) {
    sensor2Found = true;
    Serial.println("OK!");
  } else {
    Serial.println("FAILED (may need different address)");
  }
  
  delay(500);
}

void updateDisplay() {
  u8g2.clearBuffer();
  
  // Title
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(10, 12, "IR Temp Sensors");
  
  // Divider line
  u8g2.drawHLine(0, 16, 128);
  
  u8g2.setFont(u8g2_font_ncenB08_tr);
  
  // Sensor 1
  if (sensor1Found) {
    char buf[32];
    snprintf(buf, sizeof(buf), "S1: %.1fC (%.1fC)", temp1_obj, temp1_amb);
    u8g2.drawStr(5, 32, buf);
  } else {
    u8g2.drawStr(5, 32, "S1: Not found");
  }
  
  // Sensor 2
  if (sensor2Found) {
    char buf[32];
    snprintf(buf, sizeof(buf), "S2: %.1fC (%.1fC)", temp2_obj, temp2_amb);
    u8g2.drawStr(5, 48, buf);
  } else {
    u8g2.drawStr(5, 48, "S2: Not found");
  }
  
  // Footer
  u8g2.setFont(u8g2_font_5x7_tr);
  u8g2.drawStr(5, 62, "Obj Temp (Ambient)");
  
  u8g2.sendBuffer();
}

void loop() {
  // Read sensors
  if (sensor1Found) {
    temp1_obj = mlx1.readObjectTempC();
    temp1_amb = mlx1.readAmbientTempC();
    Serial.printf("Sensor1: Obj=%.2fC Amb=%.2fC\n", temp1_obj, temp1_amb);
  }
  
  if (sensor2Found) {
    temp2_obj = mlx2.readObjectTempC();
    temp2_amb = mlx2.readAmbientTempC();
    Serial.printf("Sensor2: Obj=%.2fC Amb=%.2fC\n", temp2_obj, temp2_amb);
  }
  
  // Update display
  updateDisplay();
  
  delay(1000);  // Update every second
}

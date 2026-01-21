/*
 * ESP32 OLED Fix - Try SH1106 Driver
 * 
 * The garbled display means wrong driver/resolution.
 * This sketch uses SH1106 driver (common mislabeled OLED)
 * 
 * Wiring:
 * GND -> GND | VCC -> 3V3 | SCL -> GPIO22 | SDA -> GPIO21
 */

#include <Wire.h>
#include <U8g2lib.h>

// SH1106 128x64 - this is what many "SSD1306" OLEDs actually are!
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// ESP32 I2C Pins
#define I2C_SDA 21
#define I2C_SCL 22

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n");
  Serial.println("╔══════════════════════════════════════╗");
  Serial.println("║  ESP32 OLED Fix - SH1106 Driver      ║");
  Serial.println("╚══════════════════════════════════════╝");
  
  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  
  // Initialize U8g2
  Serial.println("Initializing SH1106...");
  u8g2.begin();
  u8g2.setContrast(255);  // Max brightness
  
  // Clear screen first
  u8g2.clearBuffer();
  u8g2.sendBuffer();
  delay(100);
  
  // Fill screen white to test
  u8g2.drawBox(0, 0, 128, 64);
  u8g2.sendBuffer();
  Serial.println("✓ Filled screen white");
  delay(1000);
  
  // Show Hello text
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB18_tr);
  u8g2.drawStr(15, 30, "Hello!");
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(10, 50, "ESP32 + SH1106");
  u8g2.sendBuffer();
  
  Serial.println("✓ Hello message displayed!");
  Serial.println("\nIf STILL garbled, try uncommenting different");
  Serial.println("driver options in the code (SSD1306 128x32, etc.)");
}

void loop() {
  // Simple heartbeat - invert every 3 seconds
  static unsigned long last = 0;
  static bool inv = false;
  
  if (millis() - last > 3000) {
    inv = !inv;
    u8x8_t *u8x8 = u8g2.getU8x8();
    u8x8_cad_SendCmd(u8x8, inv ? 0xA7 : 0xA6);
    Serial.printf("Display: %s\n", inv ? "INVERTED" : "NORMAL");
    last = millis();
  }
}

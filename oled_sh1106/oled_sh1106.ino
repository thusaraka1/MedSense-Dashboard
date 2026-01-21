/*
 * SH1106 / U8g2 Driver Test - LAST RESORT
 * 
 * Some OLEDs sold as SSD1306 are actually SH1106!
 * U8g2 library has different initialization that sometimes works
 * when Adafruit library doesn't.
 * 
 * This sketch tries MULTIPLE configurations:
 * 1. SSD1306 128x64
 * 2. SSD1306 128x32
 * 3. SH1106 128x64
 * 
 * Wiring:
 * GND -> G | VCC -> 3V3 | SCL -> D1 | SDA -> D2
 */

#include <Wire.h>
#include <U8g2lib.h>

// Try different constructors - uncomment ONE at a time to test

// Option 1: SSD1306 128x64 (most common)
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Option 2: SSD1306 128x32
// U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Option 3: SH1106 128x64 (some OLEDs are mislabeled!)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define I2C_SDA D2
#define I2C_SCL D1

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n");
  Serial.println("╔══════════════════════════════════════════╗");
  Serial.println("║   U8g2 / SH1106 Driver Test              ║");
  Serial.println("╚══════════════════════════════════════════╝");
  
  // Initialize I2C with specific pins
  Wire.begin(I2C_SDA, I2C_SCL);
  
  Serial.println("\n[INFO] Trying SH1106 driver with U8g2 library...");
  
  // Initialize display
  if (u8g2.begin()) {
    Serial.println("  ✓ U8g2 initialized successfully!");
    
    // Set maximum contrast
    u8g2.setContrast(255);
    
    // Clear screen first
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    delay(100);
    
    // Draw a filled rectangle (should light up pixels)
    u8g2.drawBox(0, 0, 128, 64);  // Fill entire screen
    u8g2.sendBuffer();
    Serial.println("  ✓ Filled screen with white");
    delay(2000);
    
    // Now show text
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB14_tr);  // Big font
    u8g2.drawStr(0, 20, "IT WORKS!");
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 40, "SH1106 Driver");
    u8g2.drawStr(0, 55, "U8g2 Library");
    u8g2.sendBuffer();
    Serial.println("  ✓ Text sent to display");
    
  } else {
    Serial.println("  ✗ U8g2 initialization FAILED!");
    Serial.println("  This likely means bad wiring or defective display.");
  }
  
  Serial.println("\n════════════════════════════════════════════");
  Serial.println("  If STILL blank, the display is likely:");
  Serial.println("  1. DEFECTIVE (most probable)");
  Serial.println("  2. Not getting power (check 3.3V with multimeter)");
  Serial.println("  3. Has a broken flex ribbon connection");
  Serial.println("════════════════════════════════════════════");
}

void loop() {
  // Invert every 2 seconds
  static unsigned long lastSwitch = 0;
  static bool inverted = false;
  
  if (millis() - lastSwitch > 2000) {
    inverted = !inverted;
    u8x8_t *u8x8 = u8g2.getU8x8();
    if (inverted) {
      u8x8_cad_SendCmd(u8x8, 0xA7);  // Invert display
    } else {
      u8x8_cad_SendCmd(u8x8, 0xA6);  // Normal display
    }
    Serial.printf("Display: %s\n", inverted ? "INVERTED" : "NORMAL");
    lastSwitch = millis();
  }
  
  // Heartbeat LED
  static unsigned long lastLed = 0;
  static bool led = false;
  if (millis() - lastLed > 500) {
    led = !led;
    digitalWrite(LED_BUILTIN, led ? LOW : HIGH);
    lastLed = millis();
  }
}

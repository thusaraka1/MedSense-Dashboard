/*
 * OLED Blank Screen Fix
 * 
 * Problem: Display initializes at 0x3C but screen stays blank
 * 
 * This sketch tries:
 * 1. Both common screen sizes (128x64 and 128x32)
 * 2. Maximum contrast setting
 * 3. Raw SSD1306 reset commands
 * 4. Force display on commands
 * 
 * Wiring:
 * GND -> G | VCC -> 3V3 | SCL -> D1 | SDA -> D2
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define I2C_SDA D2
#define I2C_SCL D1
#define OLED_ADDR 0x3C
#define OLED_RESET -1

// Try 128x32 first (many cheap OLEDs are this size even if they look like 128x64)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32  // <-- Changed to 32!

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void sendRawCommand(uint8_t cmd) {
  Wire.beginTransmission(OLED_ADDR);
  Wire.write(0x00);  // Command mode
  Wire.write(cmd);
  Wire.endTransmission();
}

void forceDisplayOn() {
  Serial.println("  Sending raw SSD1306 commands...");
  
  // Turn display OFF then ON (reset sequence)
  sendRawCommand(0xAE);  // Display OFF
  delay(10);
  
  // Set contrast to maximum
  sendRawCommand(0x81);  // Set Contrast Control
  sendRawCommand(0xFF);  // Maximum value
  
  // Set Normal Display (not inverted)
  sendRawCommand(0xA6);
  
  // Force entire display ON (for testing - all pixels lit)
  sendRawCommand(0xA5);  // Entire Display ON (ignore RAM)
  delay(2000);
  
  // Return to normal mode (follow RAM content)
  sendRawCommand(0xA4);  // Resume to RAM content display
  
  // Turn display ON
  sendRawCommand(0xAF);  // Display ON
  
  Serial.println("  ✓ Raw commands sent");
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n");
  Serial.println("╔══════════════════════════════════════════════╗");
  Serial.println("║         OLED BLANK SCREEN FIX                ║");
  Serial.println("╚══════════════════════════════════════════════╝");
  
  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);
  
  // ========== ATTEMPT 1: Try 128x32 ==========
  Serial.println("\n[ATTEMPT 1] Try 128x32 configuration...");
  
  if (display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("  ✓ Initialized as 128x32");
    
    // Set maximum contrast
    display.ssd1306_command(SSD1306_SETCONTRAST);
    display.ssd1306_command(0xFF);
    
    // Clear and fill with white
    display.clearDisplay();
    display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
    display.display();
    Serial.println("  ✓ Filled screen with white pixels");
    
    delay(2000);
    
    // Show text
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("WORKING!"));
    display.setTextSize(1);
    display.println(F("128x32 Mode"));
    display.display();
    
    Serial.println("  ✓ Text displayed");
  } else {
    Serial.println("  ✗ 128x32 init failed");
  }
  
  // ========== ATTEMPT 2: Force display on with raw commands ==========
  Serial.println("\n[ATTEMPT 2] Force display on with raw I2C commands...");
  forceDisplayOn();
  
  // ========== FINAL STATUS ==========
  Serial.println("\n════════════════════════════════════════════");
  Serial.println("  If screen is STILL blank after this:");
  Serial.println("");
  Serial.println("  1. The OLED panel might be DEAD/DEFECTIVE");
  Serial.println("  2. Check if you see ANY light from the display");
  Serial.println("     (even very faint in a dark room)");
  Serial.println("  3. Try a different OLED module if available");
  Serial.println("  4. Verify VCC is actually getting 3.3V");
  Serial.println("════════════════════════════════════════════");
}

void loop() {
  // Invert display every 2 seconds (should be very visible if working)
  static unsigned long lastSwitch = 0;
  static bool inverted = false;
  
  if (millis() - lastSwitch > 2000) {
    inverted = !inverted;
    display.invertDisplay(inverted);
    Serial.printf("Display %s\n", inverted ? "INVERTED" : "NORMAL");
    lastSwitch = millis();
  }
  
  // Heartbeat on built-in LED
  static unsigned long lastLed = 0;
  static bool led = false;
  if (millis() - lastLed > 500) {
    led = !led;
    digitalWrite(LED_BUILTIN, led ? LOW : HIGH);
    lastLed = millis();
  }
}

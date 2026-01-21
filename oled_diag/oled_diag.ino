/*
 * OLED Deep Diagnostic Sketch
 * 
 * This sketch performs comprehensive diagnostics:
 * 1. Scans I2C bus for all devices
 * 2. Tries both common OLED addresses (0x3C and 0x3D)
 * 3. Attempts to initialize and display test patterns
 * 4. Reports detailed status via Serial
 * 
 * OLED Pinout (from your images - GND VCC SCL SDA order):
 * GND -> G (Ground)
 * VCC -> 3V3 (3.3V Power)
 * SCL -> D1 (GPIO 5) - Clock
 * SDA -> D2 (GPIO 4) - Data
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// We'll try both common addresses
#define OLED_ADDR_1 0x3C
#define OLED_ADDR_2 0x3D

// Pin definitions for NodeMCU
#define I2C_SDA D2  // GPIO 4
#define I2C_SCL D1  // GPIO 5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

byte foundAddress = 0;
bool displayInitialized = false;

void setup() {
  Serial.begin(115200);
  delay(2000); // Give time for serial monitor to connect
  
  Serial.println("\n");
  Serial.println("╔══════════════════════════════════════════════════════╗");
  Serial.println("║       OLED DEEP DIAGNOSTIC - ESP8266 NodeMCU         ║");
  Serial.println("╚══════════════════════════════════════════════════════╝");
  
  // ========== STEP 1: Initialize I2C ==========
  Serial.println("\n[STEP 1] Initializing I2C Bus...");
  Serial.printf("  SDA Pin: D2 (GPIO %d)\n", I2C_SDA);
  Serial.printf("  SCL Pin: D1 (GPIO %d)\n", I2C_SCL);
  
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000); // 100kHz - slower speed for reliability
  Serial.println("  ✓ I2C initialized at 100kHz");
  
  // ========== STEP 2: Scan I2C Bus ==========
  Serial.println("\n[STEP 2] Scanning I2C Bus for Devices...");
  scanI2C();
  
  // ========== STEP 3: Try to Initialize OLED ==========
  Serial.println("\n[STEP 3] Attempting OLED Initialization...");
  
  if (foundAddress != 0) {
    // Try the found address
    Serial.printf("  Trying detected address 0x%02X...\n", foundAddress);
    if (tryInitDisplay(foundAddress)) {
      displayInitialized = true;
    }
  } else {
    // Try both common addresses
    Serial.println("  No device found. Trying common addresses anyway...");
    
    Serial.printf("  Trying address 0x%02X...\n", OLED_ADDR_1);
    if (tryInitDisplay(OLED_ADDR_1)) {
      displayInitialized = true;
      foundAddress = OLED_ADDR_1;
    } else {
      Serial.printf("  Trying address 0x%02X...\n", OLED_ADDR_2);
      if (tryInitDisplay(OLED_ADDR_2)) {
        displayInitialized = true;
        foundAddress = OLED_ADDR_2;
      }
    }
  }
  
  // ========== STEP 4: Display Test Pattern ==========
  Serial.println("\n[STEP 4] Display Test Pattern...");
  if (displayInitialized) {
    showTestPattern();
    Serial.println("  ✓ Test pattern sent to display");
  } else {
    Serial.println("  ✗ Cannot show test pattern - display not initialized");
  }
  
  // ========== FINAL REPORT ==========
  Serial.println("\n╔══════════════════════════════════════════════════════╗");
  Serial.println("║                    DIAGNOSTIC REPORT                  ║");
  Serial.println("╚══════════════════════════════════════════════════════╝");
  
  if (displayInitialized) {
    Serial.println("  STATUS: ✓ DISPLAY INITIALIZED SUCCESSFULLY!");
    Serial.printf("  ADDRESS: 0x%02X\n", foundAddress);
    Serial.println("\n  If display is STILL BLANK, check:");
    Serial.println("  1. Is VCC connected to 3V3 (not 5V)?");
    Serial.println("  2. Are all connections firmly seated?");
    Serial.println("  3. Try pressing the RST button on NodeMCU");
    Serial.println("  4. The display might be defective");
  } else {
    Serial.println("  STATUS: ✗ DISPLAY INITIALIZATION FAILED!");
    Serial.println("\n  TROUBLESHOOTING:");
    if (foundAddress == 0) {
      Serial.println("  • No I2C device detected at all!");
      Serial.println("  • CHECK WIRING IMMEDIATELY:");
      Serial.println("    - Verify GND is connected to G");
      Serial.println("    - Verify VCC is connected to 3V3");
      Serial.println("    - Verify SCL is connected to D1");
      Serial.println("    - Verify SDA is connected to D2");
      Serial.println("  • Are you using jumper wires? Check for broken wires.");
      Serial.println("  • Is the OLED module soldered properly?");
    } else {
      Serial.printf("  • Device found at 0x%02X but failed to init\n", foundAddress);
      Serial.println("  • This could be a different I2C device");
      Serial.println("  • Or the OLED might need 128x32 config instead of 128x64");
    }
  }
  
  Serial.println("\n══════════════════════════════════════════════════════════");
}

void scanI2C() {
  byte error, address;
  int deviceCount = 0;
  
  Serial.println("  Scanning addresses 0x01 to 0x7F...");
  
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.printf("  ✓ Device found at address 0x%02X", address);
      
      // Identify common devices
      if (address == 0x3C || address == 0x3D) {
        Serial.print(" (likely SSD1306 OLED)");
        foundAddress = address;
      } else if (address >= 0x50 && address <= 0x57) {
        Serial.print(" (likely EEPROM)");
      } else if (address == 0x27 || address == 0x3F) {
        Serial.print(" (likely LCD I2C adapter)");
      } else if (address >= 0x20 && address <= 0x27) {
        Serial.print(" (likely PCF8574 I/O expander)");
      }
      Serial.println();
      deviceCount++;
    } else if (error == 4) {
      Serial.printf("  ⚠ Unknown error at address 0x%02X\n", address);
    }
  }
  
  if (deviceCount == 0) {
    Serial.println("  ✗ NO I2C DEVICES FOUND!");
    Serial.println("    This usually means a wiring problem.");
  } else {
    Serial.printf("  Total devices found: %d\n", deviceCount);
  }
}

bool tryInitDisplay(byte addr) {
  // Try to initialize with the given address
  if (display.begin(SSD1306_SWITCHCAPVCC, addr)) {
    Serial.printf("  ✓ Display initialized at 0x%02X!\n", addr);
    return true;
  } else {
    Serial.printf("  ✗ Failed to initialize at 0x%02X\n", addr);
    return false;
  }
}

void showTestPattern() {
  // Clear buffer
  display.clearDisplay();
  
  // Fill entire screen white to make it obvious if it works
  display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.display();
  delay(1000);
  
  // Now show text
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("=== OLED WORKING ==="));
  display.println();
  display.setTextSize(2);
  display.println(F("SUCCESS!"));
  display.setTextSize(1);
  display.println();
  display.printf("Addr: 0x%02X", foundAddress);
  display.println();
  display.println(F("SDA:D2 SCL:D1"));
  display.display();
}

void loop() {
  // Blink the display to make any display activity visible
  if (displayInitialized) {
    static unsigned long lastBlink = 0;
    static bool inverted = false;
    
    if (millis() - lastBlink > 2000) {
      inverted = !inverted;
      display.invertDisplay(inverted);
      lastBlink = millis();
    }
  }
  
  // Also blink onboard LED as heartbeat
  static unsigned long lastLed = 0;
  static bool ledState = false;
  if (millis() - lastLed > 500) {
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState ? LOW : HIGH);
    lastLed = millis();
  }
}

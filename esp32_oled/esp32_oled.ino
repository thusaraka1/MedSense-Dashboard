/*
 * ESP32 WROOM - OLED Display Test
 * 
 * ESP32 Default I2C Pins:
 * SDA -> GPIO 21
 * SCL -> GPIO 22
 * 
 * Wiring:
 * GND -> GND
 * VCC -> 3V3
 * SCL -> GPIO 22
 * SDA -> GPIO 21
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64  // Try 64 first, change to 32 if needed
#define OLED_RESET -1
#define OLED_ADDR 0x3C

// ESP32 Default I2C Pins
#define I2C_SDA 21
#define I2C_SCL 22

// ESP32 built-in LED (varies by board, GPIO 2 is common)
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void scanI2C() {
  Serial.println("  Scanning I2C bus...");
  byte error, address;
  int deviceCount = 0;
  
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.printf("  ✓ Device found at 0x%02X", address);
      if (address == 0x3C || address == 0x3D) {
        Serial.print(" (OLED)");
      }
      Serial.println();
      deviceCount++;
    }
  }
  
  if (deviceCount == 0) {
    Serial.println("  ✗ No I2C devices found - CHECK WIRING!");
  } else {
    Serial.printf("  Found %d device(s)\n", deviceCount);
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n");
  Serial.println("╔══════════════════════════════════════════╗");
  Serial.println("║     ESP32 WROOM - OLED Display Test      ║");
  Serial.println("╚══════════════════════════════════════════╝");
  
  // Print chip info
  Serial.println("\n[INFO] ESP32 Chip Info:");
  Serial.printf("  Chip Model: %s\n", ESP.getChipModel());
  Serial.printf("  Cores: %d\n", ESP.getChipCores());
  Serial.printf("  CPU Freq: %d MHz\n", ESP.getCpuFreqMHz());
  
  // Initialize I2C
  Serial.println("\n[STEP 1] Initialize I2C");
  Serial.printf("  SDA: GPIO %d\n", I2C_SDA);
  Serial.printf("  SCL: GPIO %d\n", I2C_SCL);
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);  // 100kHz for reliability
  Serial.println("  ✓ I2C initialized");
  
  // Scan I2C
  Serial.println("\n[STEP 2] Scan I2C Bus");
  scanI2C();
  
  // Initialize OLED
  Serial.println("\n[STEP 3] Initialize OLED Display");
  Serial.printf("  Trying address 0x%02X...\n", OLED_ADDR);
  
  if (display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("  ✓ OLED initialized!");
    
    // Set max contrast
    display.ssd1306_command(SSD1306_SETCONTRAST);
    display.ssd1306_command(0xFF);
    
    // Clear and fill white
    Serial.println("\n[STEP 4] Display Test");
    display.clearDisplay();
    display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
    display.display();
    Serial.println("  ✓ Filled screen white");
    delay(2000);
    
    // Show text
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 5);
    display.println(F("Hello!"));
    display.setTextSize(1);
    display.setCursor(10, 40);
    display.println(F("ESP32 + OLED Working"));
    display.setCursor(10, 52);
    display.println(F("SDA:21 SCL:22"));
    display.display();
    Serial.println("  ✓ Hello message displayed");
    
  } else {
    Serial.println("  ✗ OLED initialization FAILED!");
    
    // Try alternate address
    Serial.println("  Trying address 0x3D...");
    if (display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
      Serial.println("  ✓ OLED initialized at 0x3D!");
    } else {
      Serial.println("  ✗ Also failed at 0x3D");
    }
  }
  
  Serial.println("\n════════════════════════════════════════════");
  Serial.println("  ESP32 OLED Test Complete");
  Serial.println("════════════════════════════════════════════");
}

void loop() {
  // Invert display every 2 seconds
  static unsigned long lastSwitch = 0;
  static bool inverted = false;
  
  if (millis() - lastSwitch > 2000) {
    inverted = !inverted;
    display.invertDisplay(inverted);
    Serial.printf("Display: %s\n", inverted ? "INVERTED" : "NORMAL");
    lastSwitch = millis();
  }
  
  // Heartbeat LED
  static unsigned long lastLed = 0;
  static bool led = false;
  if (millis() - lastLed > 500) {
    led = !led;
    digitalWrite(LED_BUILTIN, led);
    lastLed = millis();
  }
}

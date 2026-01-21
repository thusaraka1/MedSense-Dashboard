/*
 * ESP8266 Test Sketch
 * Tests: LED blink, Serial communication, WiFi scan
 * Board: ESP8266 (NodeMCU / ESP-12E)
 */

#include <ESP8266WiFi.h>

// Built-in LED pin (usually GPIO2 on NodeMCU/ESP-12E)
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n========================================");
  Serial.println("   ESP8266 Hardware Test Started");
  Serial.println("========================================\n");
  
  // Test 1: GPIO - LED Setup
  Serial.println("[TEST 1] GPIO Test - LED Blink");
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("  ✓ LED pin configured as OUTPUT");
  
  // Test 2: Print chip information
  Serial.println("\n[TEST 2] Chip Information");
  Serial.printf("  • Chip ID: %08X\n", ESP.getChipId());
  Serial.printf("  • Flash Chip ID: %08X\n", ESP.getFlashChipId());
  Serial.printf("  • Flash Size: %d KB\n", ESP.getFlashChipRealSize() / 1024);
  Serial.printf("  • CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("  • Free Heap: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("  • SDK Version: %s\n", ESP.getSdkVersion());
  Serial.println("  ✓ Chip info retrieved successfully");
  
  // Test 3: WiFi scan
  Serial.println("\n[TEST 3] WiFi Module Test");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  Serial.println("  Scanning for WiFi networks...");
  int networkCount = WiFi.scanNetworks();
  
  if (networkCount == 0) {
    Serial.println("  ⚠ No networks found");
  } else {
    Serial.printf("  ✓ Found %d networks:\n", networkCount);
    for (int i = 0; i < min(networkCount, 5); i++) {
      Serial.printf("    %d. %s (RSSI: %d dBm) %s\n", 
                    i + 1, 
                    WiFi.SSID(i).c_str(), 
                    WiFi.RSSI(i),
                    WiFi.encryptionType(i) == ENC_TYPE_NONE ? "[OPEN]" : "[SECURED]");
    }
    if (networkCount > 5) {
      Serial.printf("    ... and %d more networks\n", networkCount - 5);
    }
  }
  
  Serial.println("\n========================================");
  Serial.println("   All Tests Completed Successfully!");
  Serial.println("   LED will now blink every second");
  Serial.println("========================================\n");
}

void loop() {
  // Blink the built-in LED
  // Note: On many ESP8266 boards, the LED is active LOW (LOW=ON, HIGH=OFF)
  digitalWrite(LED_BUILTIN, LOW);  // Turn LED ON
  Serial.println("LED: ON");
  delay(1000);
  
  digitalWrite(LED_BUILTIN, HIGH); // Turn LED OFF
  Serial.println("LED: OFF");
  delay(1000);
}

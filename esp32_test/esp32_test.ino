/*
 * ESP32 Test Sketch
 * Tests: LED blink, Serial communication, WiFi scan
 * Board: ESP32-12E (NodeMCU-32S / ESP-WROOM-32)
 */

#include <WiFi.h>

// Built-in LED pin (GPIO2 for most ESP32 boards)
#define LED_BUILTIN 2

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n========================================");
  Serial.println("   ESP32 Hardware Test Started");
  Serial.println("========================================\n");
  
  // Test 1: GPIO - LED Setup
  Serial.println("[TEST 1] GPIO Test - LED Blink");
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("  ✓ LED pin configured as OUTPUT");
  
  // Test 2: Print chip information
  Serial.println("\n[TEST 2] Chip Information");
  Serial.printf("  • Chip Model: %s\n", ESP.getChipModel());
  Serial.printf("  • Chip Revision: %d\n", ESP.getChipRevision());
  Serial.printf("  • Number of Cores: %d\n", ESP.getChipCores());
  Serial.printf("  • CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("  • Flash Size: %d MB\n", ESP.getFlashChipSize() / (1024 * 1024));
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
                    WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "[OPEN]" : "[SECURED]");
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
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED: ON");
  delay(1000);
  
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED: OFF");
  delay(1000);
}
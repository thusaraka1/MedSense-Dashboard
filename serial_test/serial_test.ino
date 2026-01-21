/*
 * Serial Communication Echo Test
 * Request: Check serial communication
 * Board: ESP8266 (NodeMCU / ESP-12E)
 * 
 * Instructions:
 * 1. Open Serial Monitor (baud rate 115200)
 * 2. Type characters and press Send
 * 3. Verify that the device echoes the characters back
 */

#include <ESP8266WiFi.h>

// Built-in LED pin
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

void setup() {
  // Initialize Serial at 115200 baud
  Serial.begin(115200);
  
  // Initialize LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // Off initially (Low is On for NodeMCU)

  // Wait a moment for serial to stabilize
  delay(1000);
  
  Serial.println("\n");
  Serial.println("==============================================");
  Serial.println("   Serial Communication Check: READY");
  Serial.println("==============================================");
  Serial.println("1. Communication is working if you can read this.");
  Serial.println("2. Type something in the input box and hit Enter.");
  Serial.println("3. I will echo back whatever you type.");
  Serial.println("==============================================\n");
}

void loop() {
  // Check if data is available to read
  if (Serial.available() > 0) {
    // Read the incoming byte
    char incomingByte = Serial.read();
    
    // Echo the byte back to the Serial Monitor
    Serial.print("Received: ");
    Serial.print(incomingByte);
    Serial.print(" (ASCII: ");
    Serial.print((int)incomingByte);
    Serial.println(")");

    // Blink LED briefly to indicate data received
    digitalWrite(LED_BUILTIN, LOW);  // On
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH); // Off
  }
}

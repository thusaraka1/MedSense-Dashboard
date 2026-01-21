/*
 * I2C Scanner
 * Scans for I2C devices to verify connections and find addresses.
 * 
 * Pinout for ESP8266 NodeMCU:
 * SDA -> D2 (GPIO 4)
 * SCL -> D1 (GPIO 5)
 */

#include <ESP8266WiFi.h>
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial monitor
  
  // Initialize I2C explicitly on D2 (SDA) and D1 (SCL)
  Wire.begin(D2, D1);
  
  Serial.println("\n\nI2C Scanner to scan for devices...");
  Serial.print("SDA Pin: "); Serial.println(D2);
  Serial.print("SCL Pin: "); Serial.println(D1);
}

void loop() {
  byte error, address;
  int nDevices;
 
  Serial.println("\nScanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) 
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) 
        Serial.print("0");
      Serial.println(address, HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
 
  delay(5000); // wait 5 seconds for next scan
}

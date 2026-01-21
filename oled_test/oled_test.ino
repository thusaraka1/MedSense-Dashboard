/*
 * OLED Display Test (SSD1306)
 * Board: ESP8266 NodeMCU
 * 
 * Pinout:
 * VCC -> 3.3V
 * GND -> G
 * SCL -> D1 (GPIO 5)
 * SDA -> D2 (GPIO 4)
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_RESET     -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  // Initialize I2C with specific pins (SDA=D2=4, SCL=D1=5)
  // Although these are defaults for NodeMCU, it's good to be explicit
  Wire.begin(D2, D1);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  // Address 0x3C for 128x64 or 0x3D is common. Try 0x3C first.
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display.display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);

  // Display Text
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.println(F("Hello, User!"));
  
  display.setTextSize(2);      // Draw 2X-scale text
  display.setCursor(0, 10);
  display.println(F("ESP8266"));
  
  display.setTextSize(1);
  display.setCursor(0, 30);
  display.println(F("OLED Display Test"));
  display.println(F("SDA: D2, SCL: D1"));

  display.display();
  delay(2000);
}

void loop() {
  // Simple animation or update
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
}

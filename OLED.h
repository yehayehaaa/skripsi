#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long prevMillisOLED = 0;

void tampilOLED(String X, String Y)
{
  display.setTextSize(1.5); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Hello!");
  display.println(X);
  display.println(Y);
  display.display();
}

void timedTampilOLED(String X, String Y, long durationOLED) {
  display.clearDisplay();
  unsigned long currentMillis = millis();
  
  if (currentMillis - prevMillisOLED >= durationOLED) {

    tampilOLED(X, Y);
    
    prevMillisOLED = currentMillis;
  }
  display.clearDisplay();
}

void setupOLED() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    // for (;;); // Don't proceed, loop forever
  }

  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(10); // Pause for 10 m-seconds
  
  timedTampilOLED("Hover Board", "UM", 2000);
}

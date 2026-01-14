/*
 * CyberRoboEyesTFT Basic Example
 * 
 * This example demonstrates how to use the CyberRoboEyesTFT library
 * to display animated robot eyes on a TFT display.
 * 
 * Features demonstrated:
 * - Basic eye setup and display
 * - Auto-blink animation
 * - Idle mode (random eye movement)
 * - Button-controlled mood changes (DEFAULT, HAPPY, ANGRY, TIRED)
 * - Color customization
 * - Mouth animation matching each mood
 * 
 * Hardware:
 * - ESP32-C6 with TFT display (or compatible)
 * - Button connected to GPIO 9 (BOOT button)
 * 
 * Created by Cyb3rPre4cher
 */

#include <TFT_eSPI.h>
#include <CyberRoboEyesTFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();  // Create TFT instance
// Create a RoboEyes instance (true = portrait, false = landscape)
TFT_RoboEyes roboEyes(tft, false, 1);  // Rotation: 1 = 180° rotated

// Mode switching
int currentMode = 0;  // 0=DEFAULT, 1=HAPPY, 2=ANGRY, 3=TIRED
const int NUM_MODES = 4;
bool lastButtonState = HIGH;

void setup() {
  tft.init();
  roboEyes.begin(100); // 100 FPS for smooth animation
  
  // Customize eye appearance
  roboEyes.setColors(TFT_WHITE, TFT_BLACK);
  roboEyes.setWidth(70, 70);
  roboEyes.setHeight(70, 70);
  roboEyes.setBorderradius(12, 12);
  roboEyes.setSpacebetween(20);
  
  // Enable animations
  roboEyes.setAutoblinker(true, 2, 1);  // Auto-blink every 2±1 seconds
  roboEyes.setIdleMode(true, 4, 0);     // Move eyes randomly every 4 seconds
  
  // Enable mouth (optional - can be toggled on/off)
  roboEyes.setMouth(true);
  
  // Setup button for mode switching
  pinMode(9, INPUT_PULLUP);  // BOOT button for mode switching
}

void loop() {
  roboEyes.update();
  
  // Button handling for mood changes
  int buttonState = digitalRead(9);
  
  // Detect button press (falling edge HIGH -> LOW)
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Debounce
    delay(50);
    // Verify button is still pressed
    if (digitalRead(9) == LOW) {
      // Cycle to next mood
      currentMode = (currentMode + 1) % NUM_MODES;
      
      // Apply mood and color based on current mode
      switch(currentMode) {
        case 0: // DEFAULT
          roboEyes.setColors(TFT_WHITE, TFT_BLACK);
          roboEyes.setMood(DEFAULT);
          break;
        case 1: // HAPPY
          roboEyes.setColors(TFT_GREEN, TFT_BLACK);
          roboEyes.setMood(HAPPY);
          break;
        case 2: // ANGRY
          roboEyes.setColors(TFT_RED, TFT_BLACK);
          roboEyes.setMood(ANGRY);
          break;
        case 3: // TIRED
          roboEyes.setColors(TFT_BLUE, TFT_BLACK);
          roboEyes.setMood(TIRED);
          break;
      }
      
      // Wait for button release
      while(digitalRead(9) == LOW) {
        delay(10);
      }
    }
  }
  
  lastButtonState = buttonState;
}

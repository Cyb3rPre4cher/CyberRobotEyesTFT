/*
 * CyberRoboEyesTFT Basic Example
 * 
 * This is a simple example that demonstrates the basic usage of the
 * CyberRoboEyesTFT library. It automatically cycles through different
 * moods every 5 seconds without any button interaction.
 * 
 * Features demonstrated:
 * - Auto-blink animation
 * - Idle mode (random eye movement)
 * - Automatic mood cycling (DEFAULT, HAPPY, ANGRY, TIRED)
 * - Color customization per mood
 * 
 * Hardware:
 * - ESP32-C6 with TFT display (or compatible)
 * 
 * Created by Cyb3rPre4cher
 */

#include <TFT_eSPI.h>
#include <CyberRoboEyesTFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();  // Create TFT instance
// Create a RoboEyes instance (false = landscape mode)
TFT_RoboEyes roboEyes(tft, false, 1);  // Rotation: 1 = 180° rotated

// Mode cycling
int currentMode = 0;  // 0=DEFAULT, 1=HAPPY, 2=ANGRY, 3=TIRED
const int NUM_MODES = 4;
unsigned long lastModeChange = 0;
const unsigned long MODE_INTERVAL = 5000;  // Change mode every 5 seconds

void setup() {
  tft.init();
  roboEyes.begin(60);  // 60 FPS
  
  // Customize eye appearance
  roboEyes.setColors(TFT_WHITE, TFT_BLACK);
  roboEyes.setWidth(70, 70);
  roboEyes.setHeight(70, 70);
  roboEyes.setBorderradius(12, 12);
  roboEyes.setSpacebetween(20);
  
  // Enable animations
  roboEyes.setAutoblinker(true, 2, 1);  // Auto-blink every 2±1 seconds
  roboEyes.setIdleMode(true, 4, 0);     // Move eyes randomly every 4 seconds
  roboEyes.setMouth(false);  // No mouth in basic demo
  
  // Start with default mood
  roboEyes.setMood(DEFAULT);
}

void loop() {
  roboEyes.update();
  
  // Auto-cycle through moods every 5 seconds
  if (millis() - lastModeChange >= MODE_INTERVAL) {
    lastModeChange = millis();
    currentMode = (currentMode + 1) % NUM_MODES;
    
    // Apply mood based on current mode
    switch(currentMode) {
      case 0: // DEFAULT
        roboEyes.setMood(DEFAULT);
        roboEyes.setCuriosity(true);
        break;
      case 1: // HAPPY
        roboEyes.setMood(HAPPY);
        break;
      case 2: // ANGRY
        roboEyes.setMood(ANGRY);
        break;
      case 3: // TIRED
        roboEyes.setMood(TIRED);
        break;
    }
  }
}

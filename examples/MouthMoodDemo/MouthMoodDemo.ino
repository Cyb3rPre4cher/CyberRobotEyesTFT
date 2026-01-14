/*
 * CyberRoboEyesTFT Mouth Mood Demo
 * 
 * This example demonstrates the separate mouth mood control feature.
 * You can now set different moods for the eyes and mouth independently,
 * allowing for more expressive combinations.
 * 
 * Features demonstrated:
 * - Independent eye and mouth mood control
 * - setMood() for eyes (backward compatible - also sets mouth)
 * - setMouthMood() for mouth only
 * - Various mood combinations (e.g., angry eyes with happy mouth)
 * - Auto-cycling through different combinations
 * 
 * Hardware:
 * - ESP32-C6 with TFT display (or compatible)
 * 
 * Created by Cyb3rPre4cher
 */

#include <TFT_eSPI.h>
#include <CyberRoboEyesTFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_RoboEyes roboEyes(tft, false, 1);  // Landscape mode

// Mood combination presets
struct MoodCombo {
  uint8_t eyeMood;
  uint8_t mouthMood;
  const char* description;
};

MoodCombo combos[] = {
  {DEFAULT, DEFAULT, "Default - Default"},
  {HAPPY, HAPPY, "Happy - Happy"},
  {ANGRY, ANGRY, "Angry - Angry"},
  {TIRED, TIRED, "Tired - Tired"},
  {DEFAULT, SMIRK, "Default - Smirk"},
  {ANGRY, HAPPY, "Angry - Happy (ironic)"},
  {HAPPY, ANGRY, "Happy - Angry (upset)"},
  {TIRED, HAPPY, "Tired - Happy (trying)"},
  {DEFAULT, HAPPY, "Default - Happy (pleasant)"},
  {ANGRY, SMIRK, "Angry - Smirk (sassy)"},
};

int numCombos = sizeof(combos) / sizeof(MoodCombo);
int currentCombo = 0;
unsigned long lastChange = 0;
const unsigned long changeInterval = 3000; // 3 seconds per mood

void setup() {
  Serial.begin(115200);
  tft.init();
  roboEyes.begin(60);
  
  // Configure eye appearance
  roboEyes.setColors(TFT_WHITE, TFT_BLACK);
  roboEyes.setWidth(70, 70);
  roboEyes.setHeight(70, 70);
  roboEyes.setBorderradius(12, 12);
  roboEyes.setSpacebetween(20);
  
  // Enable mouth
  roboEyes.setMouth(true);
  
  // Enable animations
  roboEyes.setAutoblinker(true, 3, 1);
  roboEyes.setIdleMode(true, 4, 1);
  
  Serial.println("Mouth Mood Demo Started");
  Serial.println("Cycling through different eye/mouth combinations...");
}

void loop() {
  roboEyes.update();
  
  // Auto-cycle through mood combinations
  if (millis() - lastChange > changeInterval) {
    // Set eye mood (this also sets mouth mood by default)
    roboEyes.setMood(combos[currentCombo].eyeMood);
    
    // Override mouth mood independently
    roboEyes.setMouthMood(combos[currentCombo].mouthMood);
    
    // Update color based on eye mood
    switch(combos[currentCombo].eyeMood) {
      case HAPPY:
        roboEyes.setColors(TFT_GREEN, TFT_BLACK);
        break;
      case ANGRY:
        roboEyes.setColors(TFT_RED, TFT_BLACK);
        break;
      case TIRED:
        roboEyes.setColors(TFT_BLUE, TFT_BLACK);
        break;
      default:
        roboEyes.setColors(TFT_WHITE, TFT_BLACK);
        break;
    }
    
    Serial.print("Combo ");
    Serial.print(currentCombo + 1);
    Serial.print("/");
    Serial.print(numCombos);
    Serial.print(": ");
    Serial.println(combos[currentCombo].description);
    
    currentCombo = (currentCombo + 1) % numCombos;
    lastChange = millis();
  }
}

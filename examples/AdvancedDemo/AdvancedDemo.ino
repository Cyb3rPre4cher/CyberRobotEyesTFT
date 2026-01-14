/*
 * CyberRoboEyesTFT Advanced Demo
 * 
 * This example demonstrates advanced features of the CyberRoboEyesTFT library
 * with interactive button controls and multiple animation modes.
 * 
 * Features demonstrated:
 * - Auto-blink animation
 * - Idle mode (random eye movement)
 * - Button-controlled mood changes (DEFAULT, HAPPY, ANGRY, TIRED, TACHOMETER)
 * - Double-click for mood-specific animations
 * - Long-press for glitch effect (horizontal + vertical flicker)
 * - Independent mouth mood control
 * - Color customization per mood
 * - Tachometer mode with animated needle
 * - Wink animation with temporary mood change
 * 
 * Hardware:
 * - ESP32-C6 with TFT display (or compatible)
 * - Button connected to GPIO 9 (BOOT button)
 * 
 * Button Controls:
 * - Single click: Switch mood/mode
 * - Double-click: Trigger animation (varies by mode)
 * - Long press (>500ms): Glitch effect (flicker)
 * 
 * Created by Cyb3rPre4cher
 */

#include <TFT_eSPI.h>
#include <CyberRoboEyesTFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();  // Create TFT instance
// Create a RoboEyes instance (true = portrait, false = landscape)
TFT_RoboEyes roboEyes(tft, false, 1);  // Rotation: 1 = 180° rotated

// Mode switching
int currentMode = 0;  // 0=DEFAULT, 1=HAPPY, 2=ANGRY, 3=TIRED, 4=TACHOMETER
const int NUM_MODES = 5;
bool lastButtonState = HIGH;

// Double-click detection
unsigned long lastButtonPress = 0;
unsigned long doubleClickTime = 300;  // Max time between clicks in ms
bool waitingForSecondClick = false;

// Long-press detection for glitch effect
unsigned long buttonPressStart = 0;
unsigned long longPressTime = 500;  // 500ms for long press
bool isLongPress = false;
bool wasLongPress = false;

// Tachometer animation
float tachSpeed = 0;
unsigned long lastTachUpdate = 0;
bool tachAccelerating = true;

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
  roboEyes.setMouth(true); 

  // Setup button for mode switching
  pinMode(9, INPUT_PULLUP);  // BOOT button for mode switching
}

void loop() {
  roboEyes.update();
  
  // Tachometer animation in mode 4
  if (currentMode == 4 && millis() - lastTachUpdate > 50) {
    if (tachAccelerating) {
      tachSpeed += 2.0;
      if (tachSpeed >= 100) {
        tachSpeed = 100;
        tachAccelerating = false;
      }
    } else {
      tachSpeed -= 1.5;
      if (tachSpeed <= 0) {
        tachSpeed = 0;
        tachAccelerating = true;
      }
    }
    roboEyes.setTachometerSpeed(tachSpeed);
    lastTachUpdate = millis();
  }
  
  // Button handling for mood changes
  int buttonState = digitalRead(9);
  
  // Long-press detection and glitch effect
  if (buttonState == LOW) {
    // Button is currently pressed
    if (lastButtonState == HIGH) {
      // Just pressed - start timer, don't process click yet
      buttonPressStart = millis();
      isLongPress = false;
      wasLongPress = false;  // Reset immediately on new press
    } else {
      // Still pressed - check if long press threshold reached
      if (!isLongPress && (millis() - buttonPressStart) >= longPressTime) {
        isLongPress = true;
        wasLongPress = true;
        waitingForSecondClick = false;  // Cancel any pending click detection
        // Activate glitch effect with explicit amplitude
        roboEyes.setHFlicker(true, 10);  // Horizontal flicker with amplitude 10
        roboEyes.setVFlicker(true, 15);  // Vertical flicker with amplitude 15
      }
    }
  } else if (lastButtonState == LOW) {
    // Button just released
    if (isLongPress) {
      // Was a long press - just deactivate glitch
      roboEyes.setHFlicker(false, 0);
      roboEyes.setVFlicker(false, 0);
      isLongPress = false;
      // wasLongPress stays true to prevent click processing
    } else if (!wasLongPress) {
      // Was a short press - process click logic
      unsigned long pressDuration = millis() - buttonPressStart;
      
      // Check if this is a double-click
      if (waitingForSecondClick && (millis() - lastButtonPress) < doubleClickTime) {
        // Double-click detected - trigger animation based on mode
        waitingForSecondClick = false;
        
        switch(currentMode) {
          case 0: // DEFAULT - laugh
            roboEyes.anim_laugh();
            break;
          case 1: // HAPPY - wink with left eye
            // Pause auto-blink temporarily
            roboEyes.setAutoblinker(false);
            // Ensure eyes are fully open first
            roboEyes.open(true, true);
            roboEyes.update();
            delay(100);
            // Wink with left eye and change mouth to smirk
            roboEyes.setMouthMood(SMIRK);
            roboEyes.close(true, false);
            roboEyes.update();
            delay(300);
            roboEyes.open(true, false);
            roboEyes.update();
            delay(100);
            // Re-enable auto-blink and restore mouth
            roboEyes.setAutoblinker(true, 2, 1);
            roboEyes.setMouthMood(HAPPY);
            break;
          case 2: // ANGRY - laugh
            roboEyes.anim_laugh();
            break;
          case 3: // TIRED - confused
            roboEyes.anim_confused();
            break;
          case 4: // TACHOMETER - no animation
            break;
        }
      } else {
        // First click - wait for potential second click
        waitingForSecondClick = true;
        lastButtonPress = millis();
      }
    }
  }
  
  // Check if double-click timeout expired - then it was a single click
  if (waitingForSecondClick && !wasLongPress && (millis() - lastButtonPress) >= doubleClickTime) {
    waitingForSecondClick = false;
    
    // Single click - cycle to next mood
    currentMode = (currentMode + 1) % NUM_MODES;
    
    // Apply mood and color based on current mode
    switch(currentMode) {
      case 0: // DEFAULT
        roboEyes.setTachometerMode(false);
        roboEyes.setMouth(true);
        roboEyes.setColors(TFT_WHITE, TFT_BLACK);
        roboEyes.setMood(DEFAULT);
        roboEyes.setCuriosity(true);
        break;
      case 1: // HAPPY
        roboEyes.setTachometerMode(false);
        roboEyes.setMouth(true);
        roboEyes.setColors(TFT_GREEN, TFT_BLACK);
        roboEyes.setMood(HAPPY);
        break;
      case 2: // ANGRY
        roboEyes.setTachometerMode(false);
        roboEyes.setMouth(true);
        roboEyes.setColors(TFT_RED, TFT_BLACK);
        roboEyes.setMood(ANGRY);
        break;
      case 3: // TIRED
        roboEyes.setTachometerMode(false);
        roboEyes.setMouth(true);
        roboEyes.setColors(TFT_BLUE, TFT_BLACK);
        roboEyes.setMood(TIRED);
        break;
      case 4: // TACHOMETER
        roboEyes.setColors(TFT_BLACK, TFT_BLACK);
        roboEyes.setTachometerMode(true);
        roboEyes.setTachometerColors(TFT_YELLOW, TFT_RED);
        roboEyes.setMouth(true);
        roboEyes.setMood(SMIRK);
        tachSpeed = 0;
        tachAccelerating = true;
        break;
    }
  }
  
  lastButtonState = buttonState;
}

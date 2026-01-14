/*
 * CyberRoboEyesTFT Advanced Tachometer Example
 * 
 * Advanced tachometer demo with multiple features:
 * - Button control for manual speed adjustment
 * - Different color schemes for speed ranges (green/yellow/red zones)
 * - Auto mode with realistic acceleration curves
 * - Toggle between normal eyes and tachometer mode
 * - Cool smirk mouth with teeth in tachometer mode
 * 
 * Hardware:
 * - ESP32-C6 with TFT display
 * - BOOT button (GPIO 9) for mode switching
 * 
 * Controls:
 * - Short press: Toggle between eye mode and tachometer mode
 * - In tachometer mode: automatic acceleration simulation
 * 
 * Created by Cyb3rPre4cher
 */

#include <TFT_eSPI.h>
#include <CyberRoboEyesTFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_RoboEyes roboEyes(tft, false, 1);

// Mode control
bool tachoModeActive = true;
bool lastButtonState = HIGH;

// Speed simulation
float currentSpeed = 0;
float acceleration = 0.8;
float deceleration = 0.5;
bool isAccelerating = true;
unsigned long lastUpdate = 0;

// Color zones for different speed ranges
#define ZONE_GREEN  33   // 0-33%: Green zone (safe)
#define ZONE_YELLOW 66   // 33-66%: Yellow zone (moderate)
                         // 66-100%: Red zone (danger)

void setup() {
  Serial.begin(115200);
  tft.init();
  
  // Initialize eyes - BLACK background
  roboEyes.begin(60);
  roboEyes.setColors(TFT_BLACK, TFT_BLACK);
  roboEyes.setWidth(70, 70);
  roboEyes.setHeight(70, 70);  // Will be halved in tachometer mode
  roboEyes.setSpacebetween(20);
  
  // Start in tachometer mode
  roboEyes.setTachometerMode(true);
  roboEyes.setTachometerColors(TFT_GREEN, TFT_GREEN);
  
  // Enable cool smirk mouth with teeth for tachometer mode
  roboEyes.setMouth(true);
  roboEyes.setMood(SMIRK);
  
  // Setup button
  pinMode(9, INPUT_PULLUP);
  
  Serial.println("Advanced Tachometer Demo");
  Serial.println("Press BOOT button to toggle between modes");
  Serial.println("Tachometer mode: Active");
}

void loop() {
  // Handle button press for mode switching
  handleButton();
  
  // Update speed simulation if in tachometer mode
  if (tachoModeActive) {
    updateSpeed();
    updateColorZone();
  }
  
  // Render the display
  roboEyes.update();
}

// Handle button press for mode switching
void handleButton() {
  int buttonState = digitalRead(9);
  
  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50);  // Debounce
    if (digitalRead(9) == LOW) {
      tachoModeActive = !tachoModeActive;
      
      if (tachoModeActive) {
        Serial.println("Mode: Tachometer");
        // Reset to black eyes for tachometer
        roboEyes.setColors(TFT_BLACK, TFT_BLACK);
        roboEyes.setTachometerMode(true);
        roboEyes.setTachometerColors(TFT_GREEN, TFT_GREEN);
        // Cool smirk mouth with teeth
        roboEyes.setMouth(true);
        roboEyes.setMood(SMIRK);
        currentSpeed = 0;
      } else {
        Serial.println("Mode: Normal Eyes");
        roboEyes.setTachometerMode(false);
        roboEyes.setMouth(false);  // Disable mouth in normal mode
        roboEyes.setColors(TFT_WHITE, TFT_BLACK);
        roboEyes.setAutoblinker(true, 2, 1);
        roboEyes.setIdleMode(true, 4, 2);
        roboEyes.setMood(DEFAULT);
      }
      
      // Wait for button release
      while(digitalRead(9) == LOW) delay(10);
    }
  }
  
  lastButtonState = buttonState;
}

// Update speed with realistic acceleration/deceleration
void updateSpeed() {
  if (millis() - lastUpdate < 30) return;  // Update every 30ms
  
  if (isAccelerating) {
    currentSpeed += acceleration;
    
    // Slow down acceleration as we approach max speed (realistic)
    if (currentSpeed > 70) {
      acceleration *= 0.98;
    }
    
    if (currentSpeed >= 100) {
      currentSpeed = 100;
      isAccelerating = false;
      acceleration = 0.8;  // Reset for next cycle
      Serial.println("Max speed!");
      delay(2000);  // Pause at max speed
    }
  } else {
    currentSpeed -= deceleration;
    
    // Increase deceleration for engine braking effect
    if (currentSpeed < 30) {
      deceleration *= 1.02;
    }
    
    if (currentSpeed <= 0) {
      currentSpeed = 0;
      isAccelerating = true;
      deceleration = 0.5;  // Reset for next cycle
      Serial.println("Stopped. Accelerating...");
      delay(1500);  // Pause before next acceleration
    }
  }
  
  roboEyes.setTachometerSpeed(currentSpeed);
  lastUpdate = millis();
}

// Update needle and scale colors based on speed zone
void updateColorZone() {
  static int lastZone = -1;
  int currentZone;
  
  if (currentSpeed < ZONE_GREEN) {
    currentZone = 0;  // Green zone
    if (lastZone != 0) {
      roboEyes.setTachometerColors(TFT_GREEN, TFT_GREEN);
      Serial.println("Zone: GREEN (Safe)");
    }
  } else if (currentSpeed < ZONE_YELLOW) {
    currentZone = 1;  // Yellow zone
    if (lastZone != 1) {
      roboEyes.setTachometerColors(TFT_YELLOW, TFT_YELLOW);
      Serial.println("Zone: YELLOW (Moderate)");
    }
  } else {
    currentZone = 2;  // Red zone
    if (lastZone != 2) {
      roboEyes.setTachometerColors(TFT_RED, TFT_RED);
      Serial.println("Zone: RED (Danger!)");
    }
  }
  
  lastZone = currentZone;
}

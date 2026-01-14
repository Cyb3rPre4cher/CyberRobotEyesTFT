/*
 * CyberRoboEyesTFT Tachometer Example
 * 
 * This example demonstrates a tachometer animation where the robot eyes
 * transform into semicircular speedometer dials with a needle that moves 
 * like during acceleration.
 * 
 * Features:
 * - Eyes become semicircular (half-round) tachometer dials
 * - Black background with colored needle and scale
 * - Animated needle rotation (180-360 degrees, bottom half)
 * - Simulates acceleration and deceleration
 * - Speed scale markings
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

// Tachometer animation variables
float currentSpeed = 0;      // Current speed value (0-100)
float targetSpeed = 0;       // Target speed to reach
unsigned long lastUpdate = 0;
bool accelerating = true;

void setup() {
  Serial.begin(115200);
  tft.init();
  roboEyes.begin(60); // 60 FPS for smooth animation
  
  // Configure eyes as BLACK semicircular tachometer dials
  roboEyes.setColors(TFT_BLACK, TFT_BLACK);
  roboEyes.setWidth(70, 70);
  roboEyes.setHeight(70, 70);  // Will become semicircle (half height)
  roboEyes.setSpacebetween(20);
  
  // Disable standard animations
  roboEyes.setAutoblinker(false);
  roboEyes.setIdleMode(false);
  
  // Activate tachometer mode for semicircular shape
  roboEyes.setTachometerMode(true);
  
  Serial.println("Tachometer Demo Started");
}

void loop() {
  // Update target speed in a loop: accelerate and decelerate
  if (millis() - lastUpdate > 50) {
    if (accelerating) {
      targetSpeed += 2.0;
      if (targetSpeed >= 100) {
        targetSpeed = 100;
        accelerating = false;
        delay(1000);  // Pause at max speed
      }
    } else {
      targetSpeed -= 1.5;
      if (targetSpeed <= 0) {
        targetSpeed = 0;
        accelerating = true;
        delay(1000);  // Pause at zero speed
      }
    }
    
    // Smooth transition to target speed
    currentSpeed += (targetSpeed - currentSpeed) * 0.1;
    
    lastUpdate = millis();
  }
  
  // Update and draw the eyes
  roboEyes.update();
  
  // Draw tachometer overlay
  drawTachometer(currentSpeed);
}

// Draw tachometer overlay on both eyes
void drawTachometer(float speed) {
  // Get eye positions (center is at top of semicircle)
  int eyeLx = roboEyes.eyeLx;
  int eyeLy = roboEyes.eyeLy;
  int eyeRx = roboEyes.eyeRx;
  int eyeRy = roboEyes.eyeRy;
  int eyeWidth = roboEyes.eyeLwidthCurrent;
  
  // Calculate center points (at top edge for semicircle)
  int leftCenterX = eyeLx + eyeWidth / 2;
  int leftCenterY = eyeLy;  // Top of the semicircle
  int rightCenterX = eyeRx + eyeWidth / 2;
  int rightCenterY = eyeRy;  // Top of the semicircle
  
  // Draw scale markings on both dials
  drawScaleMarkings(leftCenterX, leftCenterY, eyeWidth / 2);
  if (!roboEyes.cyclops) {
    drawScaleMarkings(rightCenterX, rightCenterY, eyeWidth / 2);
  }
  
  // Draw needle on both dials
  drawNeedle(leftCenterX, leftCenterY, eyeWidth / 2, speed);
  if (!roboEyes.cyclops) {
    drawNeedle(rightCenterX, rightCenterY, eyeWidth / 2, speed);
  }
}

// Draw scale markings around the dial (bottom semicircle only)
void drawScaleMarkings(int centerX, int centerY, int radius) {
  // Draw markings from 180° to 360° (bottom half)
  for (int i = 0; i <= 10; i++) {
    float angle = 180 + (i * 18);  // 180° / 10 segments = 18° per segment
    float angleRad = angle * PI / 180.0;
    
    // Outer point of marking
    int x1 = centerX + (radius - 5) * cos(angleRad);
    int y1 = centerY + (radius - 5) * sin(angleRad);
    
    // Inner point of marking (longer for major marks)
    int markLength = (i % 2 == 0) ? 8 : 5;
    int x2 = centerX + (radius - 5 - markLength) * cos(angleRad);
    int y2 = centerY + (radius - 5 - markLength) * sin(angleRad);
    
    // Draw line on sprite
    roboEyes.sprite->drawLine(x1, y1, x2, y2, TFT_RED);
    
    // Draw thicker line for major marks
    if (i % 2 == 0) {
      roboEyes.sprite->drawLine(x1 + 1, y1, x2 + 1, y2, TFT_RED);
    }
  }
}

// Draw the needle pointing to the current speed
void drawNeedle(int centerX, int centerY, int radius, float speed) {
  // Map speed (0-100) to angle (180° to 360°) for bottom semicircle
  float angle = 180 + (speed * 1.8);  // 180° total range
  float angleRad = angle * PI / 180.0;
  
  // Calculate needle endpoint
  int needleLength = radius - 15;
  int needleX = centerX + needleLength * cos(angleRad);
  int needleY = centerY + needleLength * sin(angleRad);
  
  // Draw needle (thick line)
  roboEyes.sprite->drawLine(centerX, centerY, needleX, needleY, TFT_YELLOW);
  roboEyes.sprite->drawLine(centerX + 1, centerY, needleX + 1, needleY, TFT_YELLOW);
  roboEyes.sprite->drawLine(centerX, centerY + 1, needleX, needleY + 1, TFT_YELLOW);
  
  // Draw center hub
  roboEyes.sprite->fillCircle(centerX, centerY, 3, TFT_YELLOW);
  roboEyes.sprite->fillCircle(centerX, centerY, 2, TFT_BLACK);
}

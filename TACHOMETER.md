# Tachometer Mode - Guide

## Overview

Tachometer mode transforms the robot eyes into **semicircular tachometer dials** (bottom half of a circle) with a needle that rises and falls like a real tachometer. The eyes remain **black** (background), only the **needle and scale are colored**.

## Appearance

- **Shape**: Semicircular (bottom half of a circle)
- **Background**: Black (like real tachometers)
- **Scale**: Colored markings from left (0%) to right (100%)
- **Needle**: Colored, rotates from 180° (left) to 360° (right)
- **Range**: 180° (bottom half only)

## Quick Start

```cpp
#include <TFT_eSPI.h>
#include <CyberRoboEyesTFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_RoboEyes roboEyes(tft, false, 1);

void setup() {
  tft.init();
  roboEyes.begin(60);
  
  // Configure eyes
  roboEyes.setColors(TFT_WHITE, TFT_BLACK);
  roboEyes.setWidth(70, 70);
  roboEyes.setHeight(70, 70);
  
  // Activate tachometer mode
  roboEyes.setTachometerMode(true);
}

void loop() {
  // Set speed (0-100)
  roboEyes.setTachometerSpeed(50);
  roboEyes.update();
}
```

## Functions

### setTachometerMode(bool enabled)

Activates or deactivates tachometer mode.

**What happens when activated:**
- Eyes become semicircular (bottom half of a circle)
- Eye height is halved
- Eyes remain black (background)
- Only needle and scale are colored
- Auto-blink is disabled
- Idle mode is disabled

```cpp
roboEyes.setTachometerMode(true);   // Activate
roboEyes.setTachometerMode(false);  // Deactivate
```

### setTachometerSpeed(float speed)

Sets the needle position (speed).

**Parameters:**
- `speed`: Value between 0 and 100
  - 0 = Needle far left (180°)
  - 50 = Needle at bottom (270°)
  - 100 = Needle far right (360°/0°)

**Important:** The library automatically interpolates between values for smooth animation.

```cpp
roboEyes.setTachometerSpeed(0);    // Minimum
roboEyes.setTachometerSpeed(50);   // Middle
roboEyes.setTachometerSpeed(100);  // Maximum
```

### setTachometerColors(uint16_t needleColor, uint16_t scaleColor)

Customizes the needle and scale colors.

**Parameters:**
- `needleColor`: Color of the needle (default: TFT_YELLOW)
- `scaleColor`: Color of the scale markings (default: TFT_RED)

```cpp
// Green for low speed
roboEyes.setTachometerColors(TFT_GREEN, TFT_GREEN);

// Yellow for moderate speed
roboEyes.setTachometerColors(TFT_YELLOW, TFT_YELLOW);

// Red for high speed
roboEyes.setTachometerColors(TFT_RED, TFT_RED);
```

## Advanced Usage

### Speed Zones with Color Change

```cpp
void updateColorZone(float speed) {
  if (speed < 33) {
    // Green zone (safe)
    roboEyes.setTachometerColors(TFT_GREEN, TFT_GREEN);
  } else if (speed < 66) {
    // Yellow zone (moderate)
    roboEyes.setTachometerColors(TFT_YELLOW, TFT_YELLOW);
  } else {
    // Red zone (danger)
    roboEyes.setTachometerColors(TFT_RED, TFT_RED);
  }
}
```

### Realistic Acceleration Curve

```cpp
float currentSpeed = 0;
float targetSpeed = 100;
float acceleration = 0.8;

void loop() {
  // Acceleration slows down at higher speeds
  if (currentSpeed < targetSpeed) {
    currentSpeed += acceleration;
    if (currentSpeed > 70) {
      acceleration *= 0.98;  // Smoother acceleration
    }
  }
  
  roboEyes.setTachometerSpeed(currentSpeed);
  roboEyes.update();
}
```

### Switching Between Normal and Tachometer Mode

```cpp
bool tachoMode = false;

void toggleMode() {
  tachoMode = !tachoMode;
  
  if (tachoMode) {
    roboEyes.setTachometerMode(true);
  } else {
    roboEyes.setTachometerMode(false);
    roboEyes.setAutoblinker(true, 2, 1);
    roboEyes.setIdleMode(true, 4, 2);
  }
}
```

## Technical Details

### Shape
- **Semicircular Eyes**: Bottom half of a circle
- **Center Point**: At the top edge of the visible half
- **Visible Range**: 180° (from left to right)

### Scale
- 11 Markings (0-10)
- Spacing: 18° per segment
- Range: 180° to 360° (bottom half)
- Major markings (0, 2, 4, 6, 8, 10): longer and thicker
- Minor markings (1, 3, 5, 7, 9): shorter

### Needle
- Length: Radius - 15 pixels
- Thickness: 3 pixels (for better visibility)
- Central Hub: 3 pixel radius
- Rotation: 180° to 360° (180° range, bottom half)

### Animation
- Interpolation factor: 0.15 (smooth transitions)
- Recommended FPS: 60 for smooth movement
- Update interval: ~16ms at 60 FPS

## Tips & Tricks

### 1. Optimal Eye Size
For best tachometer display:
```cpp
roboEyes.setWidth(70, 70);   // At least 60px recommended
roboEyes.setHeight(70, 70);
```

### 2. Cyclops Mode
Also works with a single eye:
```cpp
roboEyes.setCyclops(true);
roboEyes.setTachometerMode(true);
```

### 3. Performance
For smooth animation:
```cpp
roboEyes.begin(60);  // 60 FPS
```

### 4. Sensor Integration
Example with speed sensor:
```cpp
float sensorValue = analogRead(A0);
float speed = map(sensorValue, 0, 1023, 0, 100);
roboEyes.setTachometerSpeed(speed);
```

## Examples

The library contains three complete examples:

1. **SimpleTachometer.ino** - Simplest usage with auto-animation
2. **AdvancedTachometer.ino** - With color zones and mode switching
3. **Tachometer.ino** - Detailed manual implementation

## Troubleshooting

### Problem: Needle jumps instead of sliding
**Solution:** Use higher FPS (60 recommended)

### Problem: Scale is too small/large
**Solution:** Adjust eye size (at least 60px)

### Problem: Colors are not visible
**Solution:** Check that the main color (mainColor) has contrast

### Problem: Only one eye shows tachometer
**Solution:** Check if cyclops mode is disabled

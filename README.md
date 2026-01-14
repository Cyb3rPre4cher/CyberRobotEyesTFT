# CyberRoboEyesTFT Library

Animated robot eyes library for TFT displays using TFT_eSPI.

## Features

- 🤖 **Expressive Animations**: Multiple eye moods (Default, Happy, Angry, Tired)
- 👁️ **Flexible Display Modes**: Two-eye or cyclops mode
- 🎨 **Customizable Appearance**: Colors, sizes, border radius, and spacing
- ⚡ **Smooth Animation**: Double-buffered rendering for flicker-free display
- 🔄 **Auto-Blink**: Automatic eye blinking with configurable intervals
- 🎲 **Idle Mode**: Random eye movements for natural behavior
- 💧 **Special Animations**: Sweat drops, laugh, confused effects
- 📱 **Multi-Orientation**: Portrait and landscape support

## Hardware Requirements

- ESP32-C6 with TFT display (tested on Waveshare ESP32-C6-LCD-1.47)
- Compatible with any TFT display supported by TFT_eSPI library

## Dependencies

- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) library

## Installation

### Arduino IDE

1. Download this library as a ZIP file
2. In Arduino IDE: Sketch → Include Library → Add .ZIP Library
3. Select the downloaded ZIP file
4. Restart Arduino IDE

### Manual Installation

1. Download this library
2. Extract to your Arduino libraries folder:
   - Windows: `Documents\Arduino\libraries\`
   - Mac: `~/Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`
3. Restart Arduino IDE

## Quick Start

```cpp
#include <TFT_eSPI.h>
#include <CyberRoboEyesTFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_RoboEyes roboEyes(tft, false, 1); // landscape mode

void setup() {
  tft.init();
  roboEyes.begin(100); // 100 FPS
  
  // Customize appearance
  roboEyes.setColors(TFT_WHITE, TFT_BLACK);
  roboEyes.setWidth(70, 70);
  roboEyes.setHeight(70, 70);
  roboEyes.setBorderradius(12, 12);
  roboEyes.setSpacebetween(20);
  
  // Enable animations
  roboEyes.setAutoblinker(true, 2, 1);
  roboEyes.setIdleMode(true, 4, 0);
}

void loop() {
  roboEyes.update();
}
```

## API Reference

### Constructor

```cpp
TFT_RoboEyes(TFT_eSPI &display, bool portrait = true, int rotations = 1)
```

- `display`: Reference to TFT_eSPI instance
- `portrait`: Display orientation (true = portrait, false = landscape)
- `rotations`: Screen rotation (0-3)

### Core Methods

#### `begin(byte frameRate = 50)`
Initialize the eyes with specified frame rate (FPS).

#### `update()`
Update and render the eyes. Call this in your loop().

#### `setFramerate(byte fps)`
Set target frame rate.

### Appearance Customization

#### `setColors(uint16_t main, uint16_t background)`
Set eye and background colors.

#### `setWidth(byte leftEye, byte rightEye)`
Set width for left and right eyes.

#### `setHeight(byte leftEye, byte rightEye)`
Set height for left and right eyes.

#### `setBorderradius(byte leftEye, byte rightEye)`
Set border radius for rounded corners.

#### `setSpacebetween(int space)`
Set spacing between eyes.

### Mood & Expressions

#### `setMood(uint8_t mood)`
Set eye mood:
- `DEFAULT` - Normal expression
- `HAPPY` - Happy expression
- `ANGRY` - Angry expression
- `TIRED` - Tired expression

### Eye Positioning

#### `setPosition(uint8_t position)`
Set eye position:
- `DEFAULT` - Center
- `N` - North (top center)
- `NE` - North-East
- `E` - East (middle right)
- `SE` - South-East
- `S` - South (bottom center)
- `SW` - South-West
- `W` - West (middle left)
- `NW` - North-West

### Animations

#### `setAutoblinker(bool active, int interval = 1, int variation = 4)`
Enable/disable automatic blinking.
- `active`: Enable auto-blink
- `interval`: Base blink interval in seconds
- `variation`: Random variation range in seconds

#### `setIdleMode(bool active, int interval = 1, int variation = 3)`
Enable/disable random eye movement.
- `active`: Enable idle mode
- `interval`: Base movement interval in seconds
- `variation`: Random variation range in seconds

#### `setCuriosity(bool curiousBit)`
Enable/disable curious expression (eyebrow raise when looking sideways).

#### `setCyclops(bool cyclopsBit)`
Enable/disable single-eye (cyclops) mode.

#### `setSweat(bool sweatBit)`
Enable/disable sweat drop animation.

#### `anim_confused()`
Trigger confused animation.

#### `anim_laugh()`
Trigger laugh animation.

### Manual Control

#### `open()`
Open both eyes.

#### `close()`
Close both eyes.

#### `blink(bool left = true, bool right = true)`
Blink eyes.

#### `open(bool left, bool right)`
Open specific eye(s).

#### `close(bool left, bool right)`
Close specific eye(s).

## Examples

See the `examples/` folder for complete examples:

- **Basic**: Simple eye display with mood switching via button

## License

Created by Cyb3rPre4cher

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues.

## Credits

Based on the original RoboEyes library (https://github.com/FluxGarage/RoboEyes), the RoboEyesTFT Library (https://github.com/yousseftechdev/RoboEyesTFT/tree/main) adapted for TFT displays with TFT_eSPI and ESP32-C6 Chips.

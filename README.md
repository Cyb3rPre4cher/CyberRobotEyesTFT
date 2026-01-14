# CyberRoboEyesTFT Library

Animated robot eyes library for TFT displays using TFT_eSPI.

## Features

- 🤖 **Expressive Animations**: Multiple eye moods (Default, Happy, Angry, Tired, Smirk)
- 👁️ **Flexible Display Modes**: Two-eye or cyclops mode
- 🎨 **Customizable Appearance**: Colors, sizes, border radius, and spacing
- ⚡ **Smooth Animation**: Double-buffered rendering for flicker-free display
- 🔄 **Auto-Blink**: Automatic eye blinking with configurable intervals
- 🎲 **Idle Mode**: Random eye movements for natural behavior
- 💧 **Special Animations**: Sweat drops, laugh, confused effects
- 📱 **Multi-Orientation**: Portrait and landscape support
- 🏎️ **Tachometer Mode**: Transform eyes into semicircular speedometer dials
- 😀 **Mouth Support**: Optional mouth with independent mood control
- 🎭 **Animation Reactions**: Mouth reacts to eye animations (laugh, confused)

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
Set eye mood (also sets mouth mood by default for backward compatibility):
- `DEFAULT` - Normal expression
- `HAPPY` - Happy expression with smile
- `ANGRY` - Angry expression with frown
- `TIRED` - Tired expression with small open mouth
- `SMIRK` - Smirk expression with :D shaped mouth (perfect for tachometer mode)

**Example:**
```cpp
roboEyes.setMood(HAPPY);  // Sets both eyes and mouth to happy
```

#### `setMouthMood(uint8_t mood)`
Set mouth mood independently from eye mood. This allows creative combinations like angry eyes with a happy mouth (ironic), or happy eyes with an angry mouth (upset).

Available moods (same as eye moods):
- `DEFAULT` - Straight line
- `HAPPY` - Smiling arc (curve downward)
- `ANGRY` - Frowning arc (curve upward)
- `TIRED` - Small open oval
- `SMIRK` - :D shape with expanding arc (grows with tachometer speed)

**Example:**
```cpp
roboEyes.setMood(ANGRY);        // Eyes angry
roboEyes.setMouthMood(HAPPY);   // Mouth happy (ironic/sarcastic expression)
```

**Creative Combinations:**
```cpp
// Ironic smile
roboEyes.setMood(ANGRY);
roboEyes.setMouthMood(HAPPY);

// Trying to be happy while tired
roboEyes.setMood(TIRED);
roboEyes.setMouthMood(HAPPY);

// Cool and casual
roboEyes.setMood(DEFAULT);
roboEyes.setMouthMood(SMIRK);
```

### Mouth Control

#### `setMouth(bool enabled)`
Enable/disable mouth display.

**Example:**
```cpp
roboEyes.setMouth(true);  // Show mouth
```

**Note:** If you enable the mouth without calling `setMood()` or `setMouthMood()`, the mouth will display in DEFAULT mode (straight line).

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
Trigger confused animation (eyes flicker horizontally, mouth shifts left/right if enabled).

#### `anim_laugh()`
Trigger laugh animation (eyes wobble, mouth widens to 150% if enabled).

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

### Tachometer Mode

#### `setTachometerMode(bool enabled)`
Enable/disable tachometer mode. When enabled, the eyes transform into semicircular (half-round) speedometer dials with animated needles. The eyes remain black (background color), only the needle and scale markings are colored.

**Visual appearance:**
- Semicircular shape (bottom half of a circle, 180° arc)
- Black background (like real tachometers)
- Colored needle and scale markings
- 11 scale markings evenly distributed
- Smooth needle animation

**Effect:**
- Automatically makes eyes semicircular (half height)
- Disables auto-blink and idle mode (preserves state for when mode is disabled)
- Eyes stay black, only needle/scale are colored
- Works perfectly with SMIRK mouth for speed-reactive grin

**Example:**
```cpp
roboEyes.setColors(TFT_BLACK, TFT_BLACK);  // Black eyes
roboEyes.setTachometerMode(true);
roboEyes.setTachometerColors(TFT_YELLOW, TFT_RED);  // Colored needle/scale
roboEyes.setMouth(true);
roboEyes.setMood(SMIRK);  // Speed-reactive mouth
```

#### `setTachometerSpeed(float speed)`
Set the tachometer needle position (0-100).
- `speed`: Value from 0 to 100 representing percentage
- Smooth animation between speed changes

**Example:**
```cpp
roboEyes.setTachometerSpeed(75);  // Set to 75%
```

#### `setTachometerColors(uint16_t needleColor, uint16_t scaleColor)`
Customize tachometer needle and scale marking colors.
- `needleColor`: Color for the needle (default: TFT_YELLOW)
- `scaleColor`: Color for scale markings (default: TFT_RED)

**Example:**
```cpp
roboEyes.setTachometerColors(TFT_GREEN, TFT_GREEN);
```

## Examples

See the `examples/` folder for complete examples:

- **Basic**: Multi-mode demonstration with button control and double-click animations
- **SimpleTachometer**: Easy tachometer mode with auto-acceleration
- **AdvancedTachometer**: Advanced tachometer with color zones and mode switching
- **MouthMoodDemo**: Demonstrates independent eye and mouth mood combinations

### Example: Speed-Reactive Tachometer

```cpp
#include <TFT_eSPI.h>
#include <CyberRoboEyesTFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_RoboEyes roboEyes(tft, false, 1);

void setup() {
  tft.init();
  roboEyes.begin(60);
  
  roboEyes.setColors(TFT_BLACK, TFT_BLACK);
  roboEyes.setTachometerMode(true);
  roboEyes.setTachometerColors(TFT_YELLOW, TFT_RED);
  
  roboEyes.setMouth(true);
  roboEyes.setMood(SMIRK);  // Speed-reactive :D mouth
}

void loop() {
  roboEyes.setTachometerSpeed(50);  // 0-100
  roboEyes.update();
}
```

### Example: Independent Mouth Mood

```cpp
// Ironic expression: angry eyes with happy mouth
roboEyes.setMood(ANGRY);        // Eyes angry (red)
roboEyes.setMouthMood(HAPPY);   // Mouth smiling
roboEyes.setColors(TFT_RED, TFT_BLACK);
roboEyes.setMouth(true);
```

## License

Created by Cyb3rPre4cher

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues.

## Credits

Based on the original RoboEyes library (https://github.com/FluxGarage/RoboEyes), the RoboEyesTFT Library (https://github.com/yousseftechdev/RoboEyesTFT/tree/main) adapted for TFT displays with TFT_eSPI and ESP32-C6 Chips.

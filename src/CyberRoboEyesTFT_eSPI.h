/*
 * TFT_RoboEyes for TFT Displays V1.4 (Double-Buffered)
 * Adapted to work with the Waveshare ESP32-C6-LCD-1.47 using TFT_eSPI.
 * Supports portrait (172x320) and landscape modes, two-eye or cyclops mode,
 * color customization, and expressive animations (auto-blink, idle,
 * curious, laugh, and confused) I also added the sweat mode from the original Robot eyes Lib.
 *
 * Created by Cyb3rPre4cher
 */

#ifndef _TFT_ROBOEYES_H
#define _TFT_ROBOEYES_H

#include <TFT_eSPI.h>
// #include <TFT_eSprite.h>  // Include the sprite class header if needed

// Default color definitions (can be changed via setColors)
#define DEFAULT_BGCOLOR   TFT_BLACK
#define DEFAULT_MAINCOLOR TFT_WHITE

// Mood and position defines (same as original)
#define DEFAULT   0
#define TIRED     1
#define ANGRY     2
#define HAPPY     3
#define SMIRK     4  // Cool smirk with teeth (perfect for tachometer mode)

#define ON  1
#define OFF 0

// Predefined positions for the left eye
#define N   1  // north, top center
#define NE  2  // north-east, top right
#define E   3  // east, middle right
#define SE  4  // south-east, bottom right
#define S   5  // south, bottom center
#define SW  6  // south-west, bottom left
#define W   7  // west, middle left
#define NW  8  // north-west, top left

class TFT_RoboEyes {
  public:
    // Reference to the TFT display object
    TFT_eSPI *tft;
    
    // Off-screen drawing buffer (sprite) to reduce flicker
    TFT_eSprite *sprite;

    // Display configuration – you can update these via setScreenSize()
    int screenWidth = 320;   // effective width (set by user)
    int screenHeight = 172;  // effective height (set by user)
    int rotation = 0;        // screen rotation (0-3)
    uint16_t bgColor;        // background color for drawing overlays
    uint16_t mainColor;      // color for the eyes

    // Frame rate control
    int frameInterval;       // milliseconds per frame
    unsigned long fpsTimer;

    // Mood flags
    bool tired;
    bool angry;
    bool happy;
    bool smirk;     // cool grin with teeth
    bool curious;
    bool cyclops;   // if true, draw a single (center) eye
    bool eyeL_open;
    bool eyeR_open;

    // --- Eye Geometry Properties ---
    // Left eye dimensions
    int eyeLwidthDefault, eyeLheightDefault;
    int eyeLwidthCurrent, eyeLheightCurrent;
    int eyeLwidthNext, eyeLheightNext;
    int eyeLheightOffset;
    uint8_t eyeLborderRadiusDefault, eyeLborderRadiusCurrent, eyeLborderRadiusNext;

    // Right eye dimensions – mirror of left eye
    int eyeRwidthDefault, eyeRheightDefault;
    int eyeRwidthCurrent, eyeRheightCurrent;
    int eyeRwidthNext, eyeRheightNext;
    int eyeRheightOffset;
    uint8_t eyeRborderRadiusDefault, eyeRborderRadiusCurrent, eyeRborderRadiusNext;

    // Coordinates for left eye
    int eyeLxDefault, eyeLyDefault;
    int eyeLx, eyeLy;
    int eyeLxNext, eyeLyNext;

    // Coordinates for right eye
    int eyeRxDefault, eyeRyDefault;
    int eyeRx, eyeRy;
    int eyeRxNext, eyeRyNext;

    // Both eyes: eyelid and space parameters
    uint8_t eyelidsHeightMax;
    uint8_t eyelidsTiredHeight, eyelidsTiredHeightNext;
    uint8_t eyelidsAngryHeight, eyelidsAngryHeightNext;
    uint8_t eyelidsHappyBottomOffsetMax;
    uint8_t eyelidsHappyBottomOffset, eyelidsHappyBottomOffsetNext;
    int spaceBetweenDefault, spaceBetweenCurrent, spaceBetweenNext;

    // --- Animation Flags & Timers ---
    bool hFlicker;
    bool hFlickerAlternate;
    uint8_t hFlickerAmplitude;
    bool vFlicker;
    bool vFlickerAlternate;
    uint8_t vFlickerAmplitude;
    bool autoblinker;
    int blinkInterval;
    int blinkIntervalVariation;
    unsigned long blinktimer;
    bool idle;
    int idleInterval;
    int idleIntervalVariation;
    unsigned long idleAnimationTimer;
    bool confused;
    unsigned long confusedAnimationTimer;
    int confusedAnimationDuration;
    bool confusedToggle;
    bool laugh;
    unsigned long laughAnimationTimer;
    int laughAnimationDuration;
    bool laughToggle;

    // Animation - sweat on the forehead
    bool sweat;
    uint8_t sweatBorderradius;
    // Sweat drop 1
    int sweat1XPosInitial;
    int sweat1XPos;
    float sweat1YPos;
    int sweat1YPosMax;
    float sweat1Height;
    float sweat1Width;
    // Sweat drop 2
    int sweat2XPosInitial;
    int sweat2XPos;
    float sweat2YPos;
    int sweat2YPosMax;
    float sweat2Height;
    float sweat2Width;
    // Sweat drop 3
    int sweat3XPosInitial;
    int sweat3XPos;
    float sweat3YPos;
    int sweat3YPosMax;
    float sweat3Height;
    float sweat3Width;

    // Mouth animation
    bool mouth;                  // enable/disable mouth
    int mouthY;                  // Y position of mouth (calculated based on eyes)
    int mouthX;                  // X position of mouth (calculated based on eyes)
    int mouthWidth;              // width of mouth
    int mouthHeight;             // height of mouth
    int mouthCurvature;          // curvature for happy/sad mouth
    
    // Mouth animation variables
    int mouthWidthCurrent;       // current animated width
    int mouthOffsetX;            // horizontal offset for confused animation
    int mouthOffsetY;            // vertical offset for laugh animation
    
    // Separate mouth mood (independent from eye mood)
    bool mouthTired;             // mouth in tired mode
    bool mouthAngry;             // mouth in angry mode
    bool mouthHappy;             // mouth in happy mode
    bool mouthSmirk;             // mouth in smirk mode

    // --- New Blink State for AutoBlinker ---
    bool blinkingActive;             // indicates if a blink is in progress (closed state)
    unsigned long blinkCloseDurationTimer; // timer for how long to stay closed
    int blinkCloseDuration = 150;    // blink closed duration in milliseconds

    // --- Tachometer Mode ---
    bool tachometerMode;             // enable/disable tachometer mode
    float tachometerSpeed;           // current speed value (0-100)
    float tachometerTargetSpeed;     // target speed for smooth animation
    uint16_t tachometerNeedleColor;  // color of the needle
    uint16_t tachometerScaleColor;   // color of the scale markings
    
    // Store previous animation settings to restore after tachometer mode
    bool previousAutoblinker;
    int previousBlinkInterval;
    int previousBlinkIntervalVariation;
    bool previousIdle;
    int previousIdleInterval;
    int previousIdleIntervalVariation;

    // ---------------------------
    // Constructor
    // ---------------------------
    TFT_RoboEyes(TFT_eSPI &display, bool portrait = true, int rotations = 1) {
      tft = &display;
      // Sprite will be allocated in begin()

      // Handle orientation
      if (!portrait) {
        screenWidth = 320;
        screenHeight = 172;
        rotation = rotations;
        tft->setRotation(rotations);
      } else {
        rotation = rotations;
      }

      // Set default colors
      bgColor = DEFAULT_BGCOLOR;
      mainColor = DEFAULT_MAINCOLOR;

      // Default frame rate: 50fps
      frameInterval = 1000 / 50;
      fpsTimer = 0;

      // Initialize mood flags
      tired = angry = happy = smirk = curious = cyclops = false;
      eyeL_open = eyeR_open = false;

      // Initialize eye dimensions (default values, you can adjust later)
      eyeLwidthDefault = 36;
      eyeLheightDefault = 36;
      eyeLwidthCurrent = eyeLwidthDefault;
      eyeLheightCurrent = 1; // start closed
      eyeLwidthNext = eyeLwidthDefault;
      eyeLheightNext = eyeLheightDefault;
      eyeLheightOffset = 0;
      eyeLborderRadiusDefault = 8;
      eyeLborderRadiusCurrent = eyeLborderRadiusDefault;
      eyeLborderRadiusNext = eyeLborderRadiusDefault;

      eyeRwidthDefault = eyeLwidthDefault;
      eyeRheightDefault = eyeLheightDefault;
      eyeRwidthCurrent = eyeRwidthDefault;
      eyeRheightCurrent = 1; // start closed
      eyeRwidthNext = eyeRwidthDefault;
      eyeRheightNext = eyeRheightDefault;
      eyeRheightOffset = 0;
      eyeRborderRadiusDefault = 8;
      eyeRborderRadiusCurrent = eyeRborderRadiusDefault;
      eyeRborderRadiusNext = eyeRborderRadiusDefault;

      // Calculate default positions:
      spaceBetweenDefault = 10;
      spaceBetweenCurrent = spaceBetweenDefault;
      spaceBetweenNext = spaceBetweenDefault;
      eyeLxDefault = (screenWidth - (eyeLwidthDefault + spaceBetweenDefault + eyeRwidthDefault)) / 2;
      eyeLyDefault = (screenHeight - eyeLheightDefault) / 2;
      eyeLx = eyeLxDefault;
      eyeLy = eyeLyDefault;
      eyeLxNext = eyeLx;
      eyeLyNext = eyeLy;
      // Right eye
      eyeRxDefault = eyeLxDefault + eyeLwidthDefault + spaceBetweenDefault;
      eyeRyDefault = eyeLyDefault;
      eyeRx = eyeRxDefault;
      eyeRy = eyeRyDefault;
      eyeRxNext = eyeRx;
      eyeRyNext = eyeRy;

      eyelidsHeightMax = eyeLheightDefault / 2;
      eyelidsTiredHeight = 0;
      eyelidsTiredHeightNext = 0;
      eyelidsAngryHeight = 0;
      eyelidsAngryHeightNext = 0;
      eyelidsHappyBottomOffsetMax = (eyeLheightDefault / 2) + 3;
      eyelidsHappyBottomOffset = 0;
      eyelidsHappyBottomOffsetNext = 0;

      // Animation defaults
      hFlicker = false; hFlickerAlternate = false; hFlickerAmplitude = 2;
      vFlicker = false; vFlickerAlternate = false; vFlickerAmplitude = 10;
      autoblinker = false; blinkInterval = 1; blinkIntervalVariation = 4; blinktimer = 0;
      idle = false; idleInterval = 1; idleIntervalVariation = 3; idleAnimationTimer = 0;
      confused = false; confusedAnimationDuration = 500; confusedToggle = true;
      laugh = false; laughAnimationDuration = 1000; laughToggle = true;

      // Sweat animation defaults
      sweat = false;
      sweatBorderradius = 3;
      sweat1XPosInitial = 2; sweat1YPos = 2; sweat1YPosMax = 0; sweat1Height = 4; sweat1Width = 3;
      sweat2XPosInitial = 2; sweat2YPos = 2; sweat2YPosMax = 0; sweat2Height = 4; sweat2Width = 3;
      sweat3XPosInitial = 2; sweat3YPos = 2; sweat3YPosMax = 0; sweat3Height = 4; sweat3Width = 3;

      // Mouth defaults
      mouth = false;
      mouthWidth = 40;
      mouthHeight = 8;
      mouthCurvature = 0;
      mouthY = 0;
      mouthX = 0;
      mouthWidthCurrent = 40;
      mouthOffsetX = 0;
      mouthOffsetY = 0;
      
      // Initialize mouth mood to match eye mood (default)
      mouthTired = false;
      mouthAngry = false;
      mouthHappy = false;
      mouthSmirk = false;

      // New auto-blink state
      blinkingActive = false;
      blinkCloseDurationTimer = 0;

      // Tachometer defaults
      tachometerMode = false;
      tachometerSpeed = 0;
      tachometerTargetSpeed = 0;
      tachometerNeedleColor = TFT_YELLOW;
      tachometerScaleColor = TFT_RED;
      
      // Initialize previous settings storage
      previousAutoblinker = false;
      previousBlinkInterval = 1;
      previousBlinkIntervalVariation = 4;
      previousIdle = false;
      previousIdleInterval = 1;
      previousIdleIntervalVariation = 3;
    }

    // ---------------------------
    // Public methods
    // ---------------------------
    // Call from setup() to set up the sprite and reset the eyes.
    void begin(byte frameRate = 50) {
      // Allocate and create the sprite (off-screen buffer)
      sprite = new TFT_eSprite(tft);
      sprite->setColorDepth(8);
      sprite->createSprite(screenWidth, screenHeight);
      sprite->fillSprite(bgColor);

      eyeLheightCurrent = 1;
      eyeRheightCurrent = 1;
      setFramerate(frameRate);
    }

    // Update the display; call often (e.g., inside loop())
    void update() {
      if (millis() - fpsTimer >= frameInterval) {
        drawEyes();                // draw on the sprite
        sprite->pushSprite(0, 0);    // push the complete frame in one go
        fpsTimer = millis();
      }
    }

    // Set the target frame rate (fps)
    void setFramerate(byte fps) {
      frameInterval = 1000 / fps;
    }

    // Use this function to update the screen dimensions (e.g., when switching orientation)
    void setScreenSize(int w, int h) {
      screenWidth = w;
      screenHeight = h;
      // Recalculate default positions for centering the eyes
      eyeLxDefault = (screenWidth - (eyeLwidthDefault + spaceBetweenDefault + eyeRwidthDefault)) / 2;
      eyeLyDefault = (screenHeight - eyeLheightDefault) / 2;
      eyeRxDefault = eyeLxDefault + eyeLwidthDefault + spaceBetweenDefault;
      eyeRyDefault = eyeLyDefault;
      // Update next positions for smooth transition
      eyeLxNext = eyeLxDefault;
      eyeLyNext = eyeLyDefault;
      eyeRxNext = eyeRxDefault;
      eyeRyNext = eyeRyDefault;
      // Recreate sprite with new dimensions
      if(sprite) {
        sprite->deleteSprite();
        sprite->createSprite(screenWidth, screenHeight);
      }
    }

    // Customization methods
    void setWidth(byte leftEye, byte rightEye) {
      eyeLwidthNext = leftEye;
      eyeRwidthNext = rightEye;
      eyeLwidthDefault = leftEye;
      eyeRwidthDefault = rightEye;
    }

    void setHeight(byte leftEye, byte rightEye) {
      eyeLheightNext = leftEye;
      eyeRheightNext = rightEye;
      eyeLheightDefault = leftEye;
      eyeRheightDefault = rightEye;
    }

    void setBorderradius(byte leftEye, byte rightEye) {
      eyeLborderRadiusNext = leftEye;
      eyeRborderRadiusNext = rightEye;
      eyeLborderRadiusDefault = leftEye;
      eyeRborderRadiusDefault = rightEye;
    }

    void setSpacebetween(int space) {
      spaceBetweenNext = space;
      spaceBetweenDefault = space;
    }

    // Set mood expression (for eyes)
    void setMood(uint8_t mood) {
      switch (mood) {
        case TIRED:  tired = true; angry = false; happy = false; smirk = false; break;
        case ANGRY:  tired = false; angry = true; happy = false; smirk = false; break;
        case HAPPY:  tired = false; angry = false; happy = true; smirk = false; break;
        case SMIRK:  tired = false; angry = false; happy = false; smirk = true; break;
        default:     tired = false; angry = false; happy = false; smirk = false; break;
      }
      // Also update mouth mood to match (for backwards compatibility)
      setMouthMood(mood);
    }
    
    // Set mouth mood independently from eye mood
    void setMouthMood(uint8_t mood) {
      switch (mood) {
        case TIRED:  mouthTired = true; mouthAngry = false; mouthHappy = false; mouthSmirk = false; break;
        case ANGRY:  mouthTired = false; mouthAngry = true; mouthHappy = false; mouthSmirk = false; break;
        case HAPPY:  mouthTired = false; mouthAngry = false; mouthHappy = true; mouthSmirk = false; break;
        case SMIRK:  mouthTired = false; mouthAngry = false; mouthHappy = false; mouthSmirk = true; break;
        default:     mouthTired = false; mouthAngry = false; mouthHappy = false; mouthSmirk = false; break;
      }
    }

    // Predefined position for left eye (affects both eyes)
    void setPosition(uint8_t position) {
      switch (position) {
        case N:
          eyeLxNext = getScreenConstraint_X() / 2;
          eyeLyNext = 0;
          break;
        case NE:
          eyeLxNext = getScreenConstraint_X();
          eyeLyNext = 0;
          break;
        case E:
          eyeLxNext = getScreenConstraint_X();
          eyeLyNext = getScreenConstraint_Y() / 2;
          break;
        case SE:
          eyeLxNext = getScreenConstraint_X();
          eyeLyNext = getScreenConstraint_Y();
          break;
        case S:
          eyeLxNext = getScreenConstraint_X() / 2;
          eyeLyNext = getScreenConstraint_Y();
          break;
        case SW:
          eyeLxNext = 0;
          eyeLyNext = getScreenConstraint_Y();
          break;
        case W:
          eyeLxNext = 0;
          eyeLyNext = getScreenConstraint_Y() / 2;
          break;
        case NW:
          eyeLxNext = 0;
          eyeLyNext = 0;
          break;
        default:
          // DEFAULT (center): use the preset default positions.
          eyeLxNext = eyeLxDefault;
          eyeLyNext = eyeLyDefault;
          break;
      }
    }

    // Set auto blink feature (in seconds)
    void setAutoblinker(bool active, int interval = 1, int variation = 4) {
      autoblinker = active;
      blinkInterval = interval;
      blinkIntervalVariation = variation;
      // Reset blink timers and state when enabling
      blinktimer = millis() + (blinkInterval * 1000UL) + (random(blinkIntervalVariation) * 1000UL);
      blinkingActive = false;
    }

    // Set idle mode (random repositioning)
    void setIdleMode(bool active, int interval = 1, int variation = 3) {
      idle = active;
      idleInterval = interval;
      idleIntervalVariation = variation;
    }

    // Enable or disable curious mode
    void setCuriosity(bool curiousBit) {
      curious = curiousBit;
    }

    // Enable or disable cyclops mode
    void setCyclops(bool cyclopsBit) {
      cyclops = cyclopsBit;
    }

    // Horizontal flickering
    void setHFlicker(bool flickerBit, uint8_t amplitude = 2) {
      hFlicker = flickerBit;
      hFlickerAmplitude = amplitude;
    }

    // Vertical flickering
    void setVFlicker(bool flickerBit, uint8_t amplitude = 10) {
      vFlicker = flickerBit;
      vFlickerAmplitude = amplitude;
    }

    // Enable or disable sweat animation
    void setSweat(bool sweatBit) {
      sweat = sweatBit;
    }

    // Enable or disable mouth
    void setMouth(bool mouthBit) {
      mouth = mouthBit;
    }

    // Set custom colors for drawing
    void setColors(uint16_t main, uint16_t background) {
      mainColor = main;
      bgColor = background;
    }

    // --- Tachometer Mode ---
    // Enable or disable tachometer mode
    void setTachometerMode(bool enabled) {
      // Only make changes if the mode actually changes
      if (enabled == tachometerMode) return;
      
      tachometerMode = enabled;
      if (enabled) {
        // Save current animation settings before disabling
        previousAutoblinker = autoblinker;
        previousBlinkInterval = blinkInterval;
        previousBlinkIntervalVariation = blinkIntervalVariation;
        previousIdle = idle;
        previousIdleInterval = idleInterval;
        previousIdleIntervalVariation = idleIntervalVariation;
        
        // Set eyes to half-height for semicircle shape
        eyeLheightNext = eyeLheightDefault / 2;
        eyeRheightNext = eyeRheightDefault / 2;
        // Make bottom corners rounded
        eyeLborderRadiusNext = eyeLwidthDefault / 2;
        eyeRborderRadiusNext = eyeRwidthDefault / 2;
        // Keep eyes open
        eyeL_open = true;
        eyeR_open = true;
        // Eyes should be black (background color)
        // Only needle and scale will be colored
        // Disable conflicting animations
        setAutoblinker(false);
        setIdleMode(false);
      } else {
        // Restore normal eye height
        eyeLheightNext = eyeLheightDefault;
        eyeRheightNext = eyeRheightDefault;
        eyeLborderRadiusNext = eyeLborderRadiusDefault;
        eyeRborderRadiusNext = eyeRborderRadiusDefault;
        // Restore previous animation settings
        setAutoblinker(previousAutoblinker, previousBlinkInterval, previousBlinkIntervalVariation);
        setIdleMode(previousIdle, previousIdleInterval, previousIdleIntervalVariation);
      }
    }

    // Set tachometer speed (0-100)
    void setTachometerSpeed(float speed) {
      if (speed < 0) speed = 0;
      if (speed > 100) speed = 100;
      tachometerTargetSpeed = speed;
    }

    // Set tachometer colors
    void setTachometerColors(uint16_t needleColor, uint16_t scaleColor) {
      tachometerNeedleColor = needleColor;
      tachometerScaleColor = scaleColor;
    }

    // ---------------------------
    // Getters for screen constraints
    int getScreenConstraint_X() {
      return screenWidth - eyeLwidthCurrent - spaceBetweenCurrent - eyeRwidthCurrent;
    }
    int getScreenConstraint_Y() {
      // When mouth is enabled, reserve space for it below the eyes
      if (mouth) {
        // Calculate total space needed: eyes + mouth offset + mouth height + some buffer
        int effectiveEyeHeight = tachometerMode ? (eyeLheightDefault / 2) : eyeLheightDefault;
        int mouthOffset = tachometerMode ? 3 : 15;
        int mouthSpace = 30; // Approximate mouth height including opening
        int totalHeight = effectiveEyeHeight + mouthOffset + mouthSpace;
        return screenHeight - totalHeight;
      }
      return screenHeight - eyeLheightDefault;
    }

    // ---------------------------
    // Basic animation methods with modified close/open behavior
    void close() {
      eyeLheightNext = 1;
      eyeRheightNext = 1;
      eyeL_open = false;
      eyeR_open = false;
      eyeLborderRadiusNext = 0;
      eyeRborderRadiusNext = 0;
    }
    void open() {
      eyeL_open = true;
      eyeR_open = true;
      eyeLheightNext = eyeLheightDefault;
      eyeRheightNext = eyeRheightDefault;
      eyeLborderRadiusNext = eyeLborderRadiusDefault;
      eyeRborderRadiusNext = eyeRborderRadiusDefault;
    }
    void blink(bool left = true, bool right = true) {
      close();
      open();
    }
    void close(bool left, bool right) {
      if (left) { 
        eyeLheightNext = 1; 
        eyeL_open = false; 
        eyeLborderRadiusNext = 0;
      }
      if (right){ 
        eyeRheightNext = 1; 
        eyeR_open = false;
        eyeRborderRadiusNext = 0;
      }
    }
    void open(bool left, bool right) {
      if (left) { 
        eyeL_open = true; 
        eyeLheightNext = eyeLheightDefault;
        eyeLborderRadiusNext = eyeLborderRadiusDefault;
      }
      if (right){ 
        eyeR_open = true;
        eyeRheightNext = eyeRheightDefault;
        eyeRborderRadiusNext = eyeRborderRadiusDefault;
      }
    }

    void anim_confused() {
      confused = true;
    }
    void anim_laugh() {
      laugh = true;
    }

  private:
    // ---------------------------
    // Core drawing logic – adapts animations and draws the eyes on the sprite.
    void drawEyes() {
      // --- PRE-CALCULATIONS ---
      if (curious) {
        // Left eye gets bigger only at left edge
        if (eyeLxNext <= 10) { eyeLheightOffset = 10; }
        else { eyeLheightOffset = 0; }
        // Right eye gets bigger only at right edge
        if (eyeRxNext >= screenWidth - eyeRwidthCurrent - 10) { eyeRheightOffset = 10; }
        else { eyeRheightOffset = 0; }
      } else {
        eyeLheightOffset = 0;
        eyeRheightOffset = 0;
      }

      // Smooth eye height transition
      eyeLheightCurrent = (eyeLheightCurrent + eyeLheightNext + eyeLheightOffset) / 2;
      eyeLy += ((eyeLheightDefault - eyeLheightCurrent) / 2);
      eyeLy -= eyeLheightOffset / 2;

      eyeRheightCurrent = (eyeRheightCurrent + eyeRheightNext + eyeRheightOffset) / 2;
      eyeRy += ((eyeRheightDefault - eyeRheightCurrent) / 2);
      eyeRy -= eyeRheightOffset / 2;

      if (eyeL_open) {
        if (eyeLheightCurrent <= 1 + eyeLheightOffset) { eyeLheightNext = eyeLheightDefault; }
      }
      if (eyeR_open) {
        if (eyeRheightCurrent <= 1 + eyeRheightOffset) { eyeRheightNext = eyeRheightDefault; }
      }

      // Smooth eye width transition
      eyeLwidthCurrent = (eyeLwidthCurrent + eyeLwidthNext) / 2;
      eyeRwidthCurrent = (eyeRwidthCurrent + eyeRwidthNext) / 2;
      spaceBetweenCurrent = (spaceBetweenCurrent + spaceBetweenNext) / 2;

      // Smooth coordinate transitions
      eyeLx = (eyeLx + eyeLxNext) / 2;
      eyeLy = (eyeLy + eyeLyNext) / 2;
      // Right eye position depends on left eye pos + space + eye width
      eyeRxNext = eyeLxNext + eyeLwidthCurrent + spaceBetweenCurrent;
      eyeRyNext = eyeLyNext;
      eyeRx = (eyeRx + eyeRxNext) / 2;
      eyeRy = (eyeRy + eyeRyNext) / 2;

      // Smooth border radius transitions
      eyeLborderRadiusCurrent = (eyeLborderRadiusCurrent + eyeLborderRadiusNext) / 2;
      eyeRborderRadiusCurrent = (eyeRborderRadiusCurrent + eyeRborderRadiusNext) / 2;

      // --- MACRO ANIMATIONS ---
      if (autoblinker && !blinkingActive) {
        if (millis() >= blinktimer) {
          close();
          blinkingActive = true;
          blinkCloseDurationTimer = millis() + blinkCloseDuration; 
          blinktimer = millis() + (blinkInterval * 1000UL) + (random(blinkIntervalVariation) * 1000UL);
        }
      }
      if (blinkingActive && millis() >= blinkCloseDurationTimer) {
        open();
        blinkingActive = false;
      }

      if (laugh) {
        if (laughToggle) {
          setVFlicker(true, 3);
          laughAnimationTimer = millis();
          laughToggle = false;
        } else if (millis() >= laughAnimationTimer + laughAnimationDuration) {
          setVFlicker(false, 0);
          laughToggle = true;
          laugh = false;
        }
      }

      if (confused) {
        if (confusedToggle) {
          setHFlicker(true, 20);
          confusedAnimationTimer = millis();
          confusedToggle = false;
        } else if (millis() >= confusedAnimationTimer + confusedAnimationDuration) {
          setHFlicker(false, 0);
          confusedToggle = true;
          confused = false;
        }
      }

      if (idle) {
        if (millis() >= idleAnimationTimer) {
          // Allow eyes to reach edges for curious effect (0 to max inclusive)
          eyeLxNext = random(getScreenConstraint_X() + 1);
          eyeLyNext = random(getScreenConstraint_Y() + 1);
          idleAnimationTimer = millis() + (idleInterval * 1000UL) + (random(idleIntervalVariation) * 1000UL);
        }
      }

      if (hFlicker) {
        if (hFlickerAlternate) {
          eyeLx += hFlickerAmplitude;
          eyeRx += hFlickerAmplitude;
        } else {
          eyeLx -= hFlickerAmplitude;
          eyeRx -= hFlickerAmplitude;
        }
        hFlickerAlternate = !hFlickerAlternate;
      }

      if (vFlicker) {
        if (vFlickerAlternate) {
          eyeLy += vFlickerAmplitude;
          eyeRy += vFlickerAmplitude;
        } else {
          eyeLy -= vFlickerAmplitude;
          eyeRy -= vFlickerAmplitude;
        }
        vFlickerAlternate = !vFlickerAlternate;
      }

      if (cyclops) {
        eyeRwidthCurrent = 0;
        eyeRheightCurrent = 0;
        spaceBetweenCurrent = 0;
      }

      // --- ACTUAL DRAWINGS ---
      // Instead of clearing the TFT, clear the sprite to the background color.
      sprite->fillSprite(bgColor);

      // Draw eyes onto the sprite
      sprite->fillRoundRect(eyeLx, eyeLy, eyeLwidthCurrent, eyeLheightCurrent, eyeLborderRadiusCurrent, mainColor);
      if (!cyclops) {
        sprite->fillRoundRect(eyeRx, eyeRy, eyeRwidthCurrent, eyeRheightCurrent, eyeRborderRadiusCurrent, mainColor);
      }

      // Prepare mood transitions: tired, angry, happy
      if (tired) { 
        eyelidsTiredHeightNext = eyeLheightCurrent / 2; 
        eyelidsAngryHeightNext = 0; 
      } else { 
        eyelidsTiredHeightNext = 0; 
      }
      if (angry) { 
        eyelidsAngryHeightNext = eyeLheightCurrent / 2; 
        eyelidsTiredHeightNext = 0; 
      } else { 
        eyelidsAngryHeightNext = 0; 
      }
      if (happy) { 
        eyelidsHappyBottomOffsetNext = eyeLheightCurrent / 2; 
      } else { 
        eyelidsHappyBottomOffsetNext = 0; 
      }

      // Tired eyelids
      eyelidsTiredHeight = (eyelidsTiredHeight + eyelidsTiredHeightNext) / 2;
      if (!cyclops) {
        sprite->fillTriangle(eyeLx, eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy - 1,
                              eyeLx, eyeLy + eyelidsTiredHeight - 1, bgColor);
        sprite->fillTriangle(eyeRx, eyeRy - 1, eyeRx + eyeRwidthCurrent, eyeRy - 1,
                              eyeRx + eyeRwidthCurrent, eyeRy + eyelidsTiredHeight - 1, bgColor);
      } else {
        sprite->fillTriangle(eyeLx, eyeLy - 1, eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1,
                              eyeLx, eyeLy + eyelidsTiredHeight - 1, bgColor);
        sprite->fillTriangle(eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy - 1,
                              eyeLx + eyeLwidthCurrent, eyeLy + eyelidsTiredHeight - 1, bgColor);
      }

      // Angry eyelids
      eyelidsAngryHeight = (eyelidsAngryHeight + eyelidsAngryHeightNext) / 2;
      if (!cyclops) {
        sprite->fillTriangle(eyeLx, eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy - 1,
                              eyeLx + eyeLwidthCurrent, eyeLy + eyelidsAngryHeight - 1, bgColor);
        sprite->fillTriangle(eyeRx, eyeRy - 1, eyeRx + eyeRwidthCurrent, eyeRy - 1,
                              eyeRx, eyeRy + eyelidsAngryHeight - 1, bgColor);
      } else {
        sprite->fillTriangle(eyeLx, eyeLy - 1, eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1,
                              eyeLx + (eyeLwidthCurrent / 2), eyeLy + eyelidsAngryHeight - 1, bgColor);
        sprite->fillTriangle(eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy - 1,
                              eyeLx + eyeLwidthCurrent, eyeLy + eyelidsAngryHeight - 1, bgColor);
      }

      // Happy (bottom) eyelids
      eyelidsHappyBottomOffset = (eyelidsHappyBottomOffset + eyelidsHappyBottomOffsetNext) / 2;
      sprite->fillRoundRect(eyeLx - 1, (eyeLy + eyeLheightCurrent) - eyelidsHappyBottomOffset + 1,
                              eyeLwidthCurrent + 2, eyeLheightDefault, eyeLborderRadiusCurrent, bgColor);
      if (!cyclops) {
        sprite->fillRoundRect(eyeRx - 1, (eyeRy + eyeRheightCurrent) - eyelidsHappyBottomOffset + 1,
                              eyeRwidthCurrent + 2, eyeRheightDefault, eyeRborderRadiusCurrent, bgColor);
      }

      // Add sweat drops animation
      if (sweat) {
        // Sweat drop 1 -> left corner
        if (sweat1YPos <= sweat1YPosMax) { sweat1YPos += 0.5; } // vertical movement from initial to max
        else { sweat1XPosInitial = random(30); sweat1YPos = 2; sweat1YPosMax = (random(10) + 10); sweat1Width = 3; sweat1Height = 4; } // reset for next drop
        if (sweat1YPos <= sweat1YPosMax / 2) { sweat1Width += 0.8; sweat1Height += 0.8; } // shape grows in first half
        else { sweat1Width -= 0.2; sweat1Height -= 0.8; } // shrinks in second half
        sweat1XPos = sweat1XPosInitial - (sweat1Width / 2); // keep centered
        sprite->fillRoundRect(sweat1XPos, sweat1YPos, sweat1Width, sweat1Height, sweatBorderradius, mainColor);

        // Sweat drop 2 -> center area
        if (sweat2YPos <= sweat2YPosMax) { sweat2YPos += 0.5; }
        else { sweat2XPosInitial = random(screenWidth - 60) + 30; sweat2YPos = 2; sweat2YPosMax = (random(10) + 10); sweat2Width = 3; sweat2Height = 4; }
        if (sweat2YPos <= sweat2YPosMax / 2) { sweat2Width += 0.8; sweat2Height += 0.8; }
        else { sweat2Width -= 0.2; sweat2Height -= 0.8; }
        sweat2XPos = sweat2XPosInitial - (sweat2Width / 2);
        sprite->fillRoundRect(sweat2XPos, sweat2YPos, sweat2Width, sweat2Height, sweatBorderradius, mainColor);

        // Sweat drop 3 -> right corner
        if (sweat3YPos <= sweat3YPosMax) { sweat3YPos += 0.5; }
        else { sweat3XPosInitial = (screenWidth - 30) + (random(30)); sweat3YPos = 2; sweat3YPosMax = (random(10) + 10); sweat3Width = 3; sweat3Height = 4; }
        if (sweat3YPos <= sweat3YPosMax / 2) { sweat3Width += 0.8; sweat3Height += 0.8; }
        else { sweat3Width -= 0.2; sweat3Height -= 0.8; }
        sweat3XPos = sweat3XPosInitial - (sweat3Width / 2);
        sprite->fillRoundRect(sweat3XPos, sweat3YPos, sweat3Width, sweat3Height, sweatBorderradius, mainColor);
      }

      // Draw tachometer if enabled
      if (tachometerMode) {
        drawTachometer();
      }

      // Draw mouth if enabled
      if (mouth) {
        drawMouth();
      }
    } // end drawEyes

    // ---------------------------
    // Draw mouth based on current mood
    void drawMouth() {
      // Mouth animation reactions to eye animations
      int targetMouthOffsetX = 0;
      int targetMouthOffsetY = 0;
      
      // Determine temporary mood based on active animations
      bool tempSmirk = mouthSmirk;
      bool tempTired = mouthTired;
      bool tempHappy = mouthHappy;
      bool tempAngry = mouthAngry;
      
      // Laugh animation - change to SMIRK and wobble vertically
      if (laugh && !laughToggle) {
        tempSmirk = true;
        tempTired = false;
        tempHappy = false;
        tempAngry = false;
        // Wobble up and down like the eyes (using vFlickerAlternate if available)
        targetMouthOffsetY = (millis() / 100) % 2 == 0 ? 3 : -3;  // Simple wobble
      }
      
      // Confused animation - change to TIRED and shift left/right
      if (confused && !confusedToggle) {
        tempTired = true;
        tempSmirk = false;
        tempHappy = false;
        tempAngry = false;
        // Follow horizontal eye flicker
        targetMouthOffsetX = hFlickerAlternate ? 15 : -15;
      }
      
      // Smooth transitions for mouth animations
      mouthOffsetX = (mouthOffsetX * 3 + targetMouthOffsetX) / 4;
      mouthOffsetY = (mouthOffsetY * 3 + targetMouthOffsetY) / 4;
      
      // Calculate mouth position: centered below the eyes
      // Use current positions and heights for accurate placement
      // Much smaller offset in tachometer mode for closer positioning
      int mouthOffset = tachometerMode ? 3 : 15;
      bool isInverted = (rotation == 1 || rotation == 2);  // Upside down
      
      // In tachometer mode, eyes are half height, so use half height for positioning
      int effectiveEyeHeight = tachometerMode ? (eyeLheightDefault / 2) : eyeLheightDefault;
      
      // Calculate curious offset - subtract because eyes move UP when curious
      int curiousOffset = 0;
      if (curious && !tachometerMode) {
        if (cyclops) {
          curiousOffset = eyeLheightOffset / 2;
        } else {
          curiousOffset = max(eyeLheightOffset, eyeRheightOffset) / 2;
        }
      }
      
      if (cyclops) {
        // For cyclops mode, center under the single eye
        mouthX = eyeLx + (eyeLwidthCurrent / 2) - (mouthWidthCurrent / 2) + mouthOffsetX;
        // Use the target Y position (eyeLyNext) which doesn't change during blink
        int baseY = eyeLyNext + effectiveEyeHeight;
        mouthY = baseY + mouthOffset - curiousOffset + mouthOffsetY;
        
        // Add flicker offsets if active
        if (hFlicker) {
          mouthX += hFlickerAlternate ? hFlickerAmplitude : -hFlickerAmplitude;
        }
        if (vFlicker) {
          mouthY += vFlickerAlternate ? vFlickerAmplitude : -vFlickerAmplitude;
        }
      } else {
        // For two eyes, center between them
        int centerX = eyeLx + eyeLwidthCurrent + (spaceBetweenCurrent / 2);
        mouthX = centerX - (mouthWidthCurrent / 2) + mouthOffsetX;
        // Use the target Y position (eyeLyNext/eyeRyNext) which doesn't change during blink
        int baseY = eyeLyNext + effectiveEyeHeight;
        mouthY = baseY + mouthOffset - curiousOffset + mouthOffsetY;
        
        // Add flicker offsets if active
        if (hFlicker) {
          mouthX += hFlickerAlternate ? hFlickerAmplitude : -hFlickerAmplitude;
        }
        if (vFlicker) {
          mouthY += vFlickerAlternate ? vFlickerAmplitude : -vFlickerAmplitude;
        }
      }

      // Determine mouth color - use tachometer needle color in tachometer mode
      uint16_t mouthColor = tachometerMode ? tachometerNeedleColor : mainColor;

      // Draw different mouth shapes based on MOUTH mood (use temp variables during animations)
      if (tempSmirk) {
        // Smirk: :D shape (rotated 90°) - straight line on top, expanding arc below
        // Calculate grin intensity based on tachometer speed (0-100)
        float speedFactor = tachometerMode ? (tachometerSpeed / 100.0) : 0.5;
        
        // Dynamic mouth width - starts at 80% and grows to 130%
        int dynamicWidth = mouthWidthCurrent * (0.8 + 0.5 * speedFactor);
        int widthOffset = (mouthWidthCurrent - dynamicWidth) / 2;
        
        // D-shape expansion height - grows with speed
        int dHeight = (int)(25 * speedFactor);  // 0-25 pixels expansion
        
        // Draw straight horizontal line (top of D-shape) - thick line
        for (int i = 0; i < dynamicWidth; i++) {
          int x = mouthX + widthOffset + i;
          sprite->fillCircle(x, mouthY, 2, mouthColor);
          sprite->fillCircle(x, mouthY + 1, 2, mouthColor);
          sprite->fillCircle(x, mouthY - 1, 2, mouthColor);
        }
        
        // Draw expanding D-arc below the straight line
        if (dHeight > 2) {
          // Fill interior with dark red
          for (int i = 0; i < dynamicWidth; i++) {
            float progress = (float)i / dynamicWidth;
            // Parabolic arc - belly of the D expanding downward
            int arcDepth = (int)(dHeight * 4.0 * progress * (1 - progress));
            int bottomY = mouthY + arcDepth;
            
            // Fill from straight line down to arc
            for (int y = mouthY + 3; y < bottomY; y++) {
              sprite->drawPixel(mouthX + widthOffset + i, y, 0x1800);  // Dark red interior
            }
          }
          
          // Draw the arc outline (bottom of D)
          for (int i = 0; i < dynamicWidth; i++) {
            float progress = (float)i / dynamicWidth;
            int arcDepth = (int)(dHeight * 4.0 * progress * (1 - progress));
            int bottomY = mouthY + arcDepth;
            sprite->fillCircle(mouthX + widthOffset + i, bottomY, 2, mouthColor);
          }
        }
      } else if (tempHappy) {
        // Happy: smiling arc (curve downward = positive yOffset)
        // Draw a curved line using filled circle segments
        for (int i = 0; i < mouthWidthCurrent; i++) {
          float progress = (float)i / mouthWidthCurrent;
          // Parabolic curve for smile
          int yOffset = (int)(12 * progress * (1 - progress));
          sprite->fillCircle(mouthX + i, mouthY + yOffset, 1, mouthColor);
        }
        // Thicker line for better visibility
        for (int i = 0; i < mouthWidthCurrent; i++) {
          float progress = (float)i / mouthWidthCurrent;
          int yOffset = (int)(12 * progress * (1 - progress));
          sprite->fillCircle(mouthX + i, mouthY + yOffset + 1, 1, mouthColor);
        }
      } else if (tempAngry) {
        // Angry: frowning arc (curve upward = negative yOffset)
        for (int i = 0; i < mouthWidthCurrent; i++) {
          float progress = (float)i / mouthWidthCurrent;
          int yOffset = (int)(12 * progress * (1 - progress));
          sprite->fillCircle(mouthX + i, mouthY - yOffset, 1, mouthColor);
        }
        for (int i = 0; i < mouthWidthCurrent; i++) {
          float progress = (float)i / mouthWidthCurrent;
          int yOffset = (int)(12 * progress * (1 - progress));
          sprite->fillCircle(mouthX + i, mouthY - yOffset + 1, 1, mouthColor);
        }
      } else if (tempTired) {
        // Tired: small open mouth (oval)
        sprite->fillRoundRect(mouthX + 10, mouthY, mouthWidthCurrent - 20, mouthHeight + 4, 3, mouthColor);
      } else {
        // Default: straight line
        sprite->fillRoundRect(mouthX, mouthY, mouthWidthCurrent, 3, 1, mouthColor);
      }
    }

    // ---------------------------
    // Draw tachometer overlay on the eyes
    void drawTachometer() {
      // Smooth speed transition
      tachometerSpeed += (tachometerTargetSpeed - tachometerSpeed) * 0.15;
      
      // For semicircle tachometer, the center is at the top edge
      // of the visible half-circle
      int leftCenterX = eyeLx + eyeLwidthCurrent / 2;
      int leftCenterY = eyeLy;  // Top of the eye
      int radius = eyeLwidthCurrent / 2;
      
      // Draw left tachometer
      drawTachometerDial(leftCenterX, leftCenterY, radius);
      
      // Draw right tachometer if not in cyclops mode
      if (!cyclops) {
        int rightCenterX = eyeRx + eyeRwidthCurrent / 2;
        int rightCenterY = eyeRy;  // Top of the eye
        drawTachometerDial(rightCenterX, rightCenterY, radius);
      }
    }

    // Draw a single tachometer dial
    void drawTachometerDial(int centerX, int centerY, int radius) {
      // For semicircle tachometer, center is at the top of the visible half
      // Draw arc from 180° (left) to 0° (right) - bottom semicircle
      
      // Draw scale markings only in the bottom half (180° to 360°)
      // Map to tachometer range: left=0, bottom=50, right=100
      for (int i = 0; i <= 10; i++) {
        // Map to 180° range (from 180° to 360° / 0°)
        float angle = 180 + (i * 18);  // 180° / 10 = 18° per segment
        float angleRad = angle * PI / 180.0;
        
        // Outer point
        int x1 = centerX + (radius - 5) * cos(angleRad);
        int y1 = centerY + (radius - 5) * sin(angleRad);
        
        // Inner point (longer for major marks)
        int markLength = (i % 2 == 0) ? 8 : 5;
        int x2 = centerX + (radius - 5 - markLength) * cos(angleRad);
        int y2 = centerY + (radius - 5 - markLength) * sin(angleRad);
        
        // Draw marking
        sprite->drawLine(x1, y1, x2, y2, tachometerScaleColor);
        if (i % 2 == 0) {
          sprite->drawLine(x1 + 1, y1, x2 + 1, y2, tachometerScaleColor);
        }
      }
      
      // Draw needle - map speed 0-100 to angle 180-360°
      float angle = 180 + (tachometerSpeed * 1.8);  // Map 0-100 to 180°
      float angleRad = angle * PI / 180.0;
      int needleLength = radius - 15;
      int needleX = centerX + needleLength * cos(angleRad);
      int needleY = centerY + needleLength * sin(angleRad);
      
      // Draw thick needle
      sprite->drawLine(centerX, centerY, needleX, needleY, tachometerNeedleColor);
      sprite->drawLine(centerX + 1, centerY, needleX + 1, needleY, tachometerNeedleColor);
      sprite->drawLine(centerX, centerY + 1, needleX, needleY + 1, tachometerNeedleColor);
      
      // Draw center hub
      sprite->fillCircle(centerX, centerY, 3, tachometerNeedleColor);
      sprite->fillCircle(centerX, centerY, 2, bgColor);
    }

}; // end class TFT_RoboEyes

#endif

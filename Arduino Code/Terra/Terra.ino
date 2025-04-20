// Latest working code as of March 26 2024.

// Include Libraries
#include <TinyGPSPlus.h>
#include <Wire.h>
#include <map>
#include "Adafruit_DRV2605.h"
#include "Adafruit_GC9A01A.h" // Include TFT library here

// --- Moved from config.ino ---
const bool debugMode = false;
// Compass Sensor Setup
#define CMPS12_ADDRESS 0x60
#define ANGLE_8 1
// TFT Display Settings
#define GC9A01A_DC 33
#define GC9A01A_CS 15
#define GC9A01A_BL 12 // Same as BACKLIGHT_PIN
#define logoWidth  240
#define logoHeight 240
#define BG_COLOR GC9A01A_BLACK
#define FG_COLOR GC9A01A_WHITE
// Backlight Pin (Redundant with GC9A01A_BL but kept for clarity if used elsewhere)
#define BACKLIGHT_PIN 12
// GPS Waypoints and Trail Settings
const double startLat = 51.536286769987;
const double startLon = -0.12449799356353357;
const double stopLats[] = { 51.536862, 51.537587, 51.53831, 51.538896, 51.539512, 51.540152, 51.540788, 51.541382, 51.541977, 51.542588, 51.536286769987 };
const double stopLons[] = { -0.125599, -0.126087, -0.126477, -0.126926, -0.127397, -0.127758, -0.128196, -0.128654, -0.129082, -0.129531, -0.12449799356353357 };
const int numberOfStops = 11;
// Thresholds for triggers in meters
const int checkpointTrigger = 10;  // Distance from the checkpoint that is considered an arrival
const int vibrationTrigger = 20;   // When to trigger vibration to indicate the checkpoint is getting close
// Vibration timing
const int proximityVibrationDelayMs = 500;   // To determine the time between vibrations when close to the current stop.
// --- End Moved from config.ino ---

// --- Moved from images.ino ---
// Image Types Enum
enum ImageType {
  NONE, MY_PENDING, GOTOSTART,
  ARROW_N, ARROW_NNE, ARROW_NE, ARROW_ENE, ARROW_E, ARROW_ESE, ARROW_SE, ARROW_SSE,
  ARROW_S, ARROW_SSW, ARROW_SW, ARROW_WSW, ARROW_W, ARROW_WNW, ARROW_NW, ARROW_NNW,
  CHECKPOINT_1, CHECKPOINT_2, CHECKPOINT_3, CHECKPOINT_4, CHECKPOINT_5,
  CHECKPOINT_6, CHECKPOINT_7, CHECKPOINT_8, CHECKPOINT_9, CHECKPOINT_10
};
ImageType currentDisplayedImage = NONE; // Tracks the currently displayed image
// --- End Moved from images.ino ---

// --- Moved from navigation.ino ---
// Navigation State Enum
enum NavigationState {
  NOT_STARTED, NAVIGATING, AT_CHECKPOINT, TRAIL_ENDED
};
NavigationState navigationState = NOT_STARTED;  // Initial navigation state
unsigned long lastCheckpointTime = 0;           // Timestamp of when the last checkpoint was reached
// --- End Moved from navigation.ino ---

// --- Moved from vibration.ino ---
// Vibration settings and state
int effectNumber = 58; // Default vibration effect
bool proximityVibrationTriggered = false; // Flag if vibration is active due to proximity
unsigned long lastVibrationTime = 0;   // Tracks the last time vibration was triggered for continuous pulsing
// --- End Moved from vibration.ino ---


// TFT Display Object
Adafruit_GC9A01A tft(GC9A01A_CS, GC9A01A_DC);

// GPS Object (Declaration moved from gps.ino)
TinyGPSPlus gps;

// Vibration motor object
Adafruit_DRV2605 drv;

// Global Variables for GPS Data and State (Remaining)
int currentStop = 0; // Tracks the current target checkpoint index (1-based)
double currentLat, currentLon;
bool trailStarted = false;
bool dataReceived = false;
bool screenOn = false; // Used by display fade functions
double distance;  // Distance to the next checkpoint, updated in navigation

// NOTE: Function Prototypes section removed - Arduino IDE handles .ino file prototypes.

// Setup Function
void setup() {
  Serial.begin(115200);

  if (!debugMode) {
    // 9600 is the default baud rate for the BN800 GPS
    Serial1.begin(9600);
  }

  // Screen
  tft.begin();
  tft.setRotation(1);
  pinMode(BACKLIGHT_PIN, OUTPUT);
  digitalWrite(BACKLIGHT_PIN, LOW);


  // Vibration motor
  if (!drv.begin()) {
    Serial.println("Could not find DRV2605");
    while (1) delay(10);
  }

  drv.selectLibrary(1);
  // I2C trigger by sending 'go' command
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG);

  Wire.begin();
}

// Main Loop
void loop() {
  Serial.println("loop start");

  delay(1000);
  if (nonBlockingDelay(1000)) {
    handleGPSData();
    determineTrailStatusAndNavigate();
  }
  if (!debugMode) {
    smartDelay(1000);  // for reading the GPS module
  }
  // Continuously trigger vibration when within a certain distance of the next stop
  if (proximityVibrationTriggered && millis() - lastVibrationTime >= proximityVibrationDelayMs) {
    triggerProximityVibration();
    lastVibrationTime = millis();  // Update the last vibration time
  }
}

// GPS functions (handleGPSData, readSerialGPS, readGPS, smartDelay) moved to gps.ino

// Navigation function (determineTrailStatusAndNavigate) moved to navigation.ino

// Vibration function (triggerProximityVibration) moved to vibration.ino

// Display functions (displayImage, drawBitmap, fadeOut, fadeIn) moved to display.ino

// Navigation helper functions (calculateRelativeDirection, selectArrowImage) moved to navigation.ino

// Non blocking delay to read sensors (remains in Terra.ino)
bool nonBlockingDelay(unsigned long ms) {
  static unsigned long lastCheck = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - lastCheck >= ms) {
    lastCheck = currentMillis;
    return true;
  }
  return false;
}

// Navigation helper functions (getDistanceTo, getCardinalTo, getCourseTo) moved to navigation.ino

// Compass function (readCompass) moved to compass.ino

// Display functions (fadeOut, fadeIn) moved to display.ino
// Definitions removed from here.

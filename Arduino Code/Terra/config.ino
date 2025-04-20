// config.ino - Contains trail configuration and device settings

const bool debugMode = false;

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

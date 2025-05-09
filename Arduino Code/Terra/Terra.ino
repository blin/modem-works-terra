// Latest working code as of March 26 2024.
#include "types.h"

// Include Libraries
#include <TinyGPSPlus.h>
#include <Wire.h>
#include <map>
#include "Adafruit_DRV2605.h"
#include "Adafruit_GC9A01A.h" // Include TFT library here

// state
const bool debugMode = false;

NavigationState navigationState = NOT_STARTED;  // Initial navigation state

bool proximityVibrationTriggered = false; // Flag if vibration is active due to proximity
unsigned long lastVibrationTime = 0;   // Tracks the last time vibration was triggered for continuous pulsing

// state end

// TFT Display Object
#define GC9A01A_DC 33
#define GC9A01A_CS 15
#define GC9A01A_BL 12
Adafruit_GC9A01A tft(GC9A01A_CS, GC9A01A_DC);

// GPS Object (Declaration moved from gps.ino)
TinyGPSPlus gps;

// Vibration motor object
Adafruit_DRV2605 drv;

// Global Variables for GPS Data and State (Remaining)
int currentStop = 0; // Tracks the current target checkpoint index (1-based)
double currentLat, currentLon;
double targetLat, targetLon;
bool trailStarted = false;
bool dataReceived = false;
bool screenOn = false; // Used by display fade functions
double distance;  // Distance to the next checkpoint, updated in navigation

// Setup Function
void setup() {
  // Initialize Serial (UART0) for debugging output via USB
  Serial.begin(115200);

  // Initialize Serial1 (UART1) for communication with the GPS module (BN800)
  // Uses different pins than Serial (UART0). Check ESP32 board pinout.
  // 9600 is the default baud rate for the BN800 GPS
  Serial1.begin(9600);

  // Screen
  tft.begin();
  tft.setRotation(1);
  pinMode(GC9A01A_BL, OUTPUT);
  digitalWrite(GC9A01A_BL, LOW);


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

  displayImage(GOTOSTART);

  proximityVibrationTriggered = true;
}

const int proximityVibrationDelayMs = 500;   // To determine the time between vibrations when close to the current stop.

// Main Loop
void loop() {
  Serial.println("loop start");
  triggerProximityVibration();

  processGPSStream(gps, Serial1, 1000);
  updateLocationGlobals(gps, currentLat, currentLon, dataReceived);

  if (!locationKnown(gps)) {
    drawText(tft, Serial, "Location is unknown\nsat=%d\nwalk around to find satellites\n", gps.satellites.value());
    delay(10000);
    return;
  }


  switch (navigationState) {
    case NOT_STARTED:
      handleNotStartedState();
      navigationState = NAVIGATING;
      break;
    case NAVIGATING:
      handleNavigatingState();
      break;
    default:
      // Should not happen
      Serial.println("Error: Unknown navigation state!");
      break;
  }
  triggerProximityVibration();
}

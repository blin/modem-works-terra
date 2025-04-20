// Latest working code as of March 26 2024.

// Include Libraries
#include <TinyGPSPlus.h>
#include <Wire.h>
#include <map>
#include "Adafruit_DRV2605.h"
#include "Adafruit_GC9A01A.h" // Include TFT library here

// Configuration, images, etc., are now in separate .ino files

// Sensor Variables
// Compass Variables
unsigned char high_byte, low_byte, angle8;
char pitch, roll;
unsigned int angle16;

// TFT Display Object
// TFT Display Object
Adafruit_GC9A01A tft(GC9A01A_CS, GC9A01A_DC);

// The TinyGPSPlus object moved to gps.ino

// Vibration motor object and variables moved to vibration.ino
Adafruit_DRV2605 drv;
// int effectNumber = 58; // Moved to vibration.ino
// Vibration motor object and state moved to vibration.ino

// Thresholds and timing moved to config.ino

// Image Types Enum moved to images.ino
// enum definition removed as it's now in images.ino

// Navigation State Enum and variables moved to navigation.ino
// enum NavigationState { ... }; // Moved
// NavigationState navigationState = NOT_STARTED; // Moved
// unsigned long lastCheckpointTime = 0; // Moved

// Global Variables for GPS Data and State
int currentStop = 0; // Tracks the current target checkpoint index (1-based)
double currentLat, currentLon;
bool trailStarted = false;
bool dataReceived = false;
bool screenOn = false;
double distance;  // Distance to the next checkpoint

// Function Prototypes
void setup();
void loop();
void handleGPSData();
bool readSerialGPS();
// Function Prototypes (Arduino IDE handles .ino prototypes, but kept for reference)
// void setup(); // In Terra.ino
// void loop(); // In Terra.ino
// void handleGPSData(); // In gps.ino
// bool readSerialGPS(); // In gps.ino
// bool readGPS(); // In gps.ino
// static void smartDelay(unsigned long ms); // In gps.ino
// void determineTrailStatusAndNavigate(); // In navigation.ino
// bool nonBlockingDelay(unsigned long ms); // In Terra.ino
// void displayImage(ImageType image); // In display.ino
// void drawBitmap(const unsigned char* bitmap); // In display.ino
// void fadeOut(); // In display.ino
// void fadeIn(); // In display.ino
// double getDistanceTo(double lat, double lon); // In navigation.ino
// String getCardinalTo(double lat, double lon); // In navigation.ino
// int getCourseTo(double lat, double lon); // In navigation.ino
// int readCompass(); // In compass.ino
// void triggerProximityVibration(); // In vibration.ino
// int calculateRelativeDirection(int currentAngle, int targetAngle); // In navigation.ino
// ImageType selectArrowImage(int relativeDirection); // In navigation.ino


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
void fadeOut() {
  for (int i = 255; i >= 0; i -= 5) {
    analogWrite(BACKLIGHT_PIN, i);
    delay(10);
  }
  screenOn = false;
}

void fadeIn() {
  for (int i = 0; i <= 255; i += 5) {
    analogWrite(BACKLIGHT_PIN, i);
    delay(10);
  }
  screenOn = true;
}

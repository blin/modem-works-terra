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

// Navigation State Enum moved to navigation.ino
enum NavigationState {
  NOT_STARTED,
  NAVIGATING,
  AT_CHECKPOINT,
  TRAIL_ENDED
};

// Initialize your navigation state
NavigationState navigationState = NOT_STARTED;  // Initial navigation state
unsigned long lastCheckpointTime = 0;           // Timestamp of when the last checkpoint was reached

// Global Variables for GPS Data and State
int currentStop = 0;
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

// Navigation function moved to navigation.ino
void determineTrailStatusAndNavigate() {
  static double lastDistance = -1;

  // Obtain the target coordinates based on the trail's current status
  double targetLat = !trailStarted ? startLat : stopLats[currentStop - 1];
  double targetLon = !trailStarted ? startLon : stopLons[currentStop - 1];

  // Calculate the current distance and direction to the target
  double distance = getDistanceTo(targetLat, targetLon);
  String cardinal = getCardinalTo(targetLat, targetLon);
  int targetAngle = getCourseTo(targetLat, targetLon);  // Assume implementation exists
  int currentAngle = readCompass();

  // Calculate the relative direction for navigation
  int relativeDirection = calculateRelativeDirection(currentAngle, targetAngle);

  if (navigationState == NOT_STARTED) {
    if (!dataReceived) {
      displayImage(MY_PENDING);  // Show MY_PENDING only when waiting for the first GPS data
    } else {
      Serial.println("Please proceed to the start of the trail.");
      displayImage(GOTOSTART);  // Now we're sure we've received data, show GOTOSTART
    }

    // Transition to navigating state once within close range to the start and data has been received
    if (dataReceived && distance <= checkpointTrigger) {
      trailStarted = true;
      currentStop = 1;
      Serial.println("Trail started. Heading to Stop 1.");
      navigationState = NAVIGATING;
    }
    return;  // Continue to skip rest of the function logic when NOT_STARTED
  }

  // Only update navigation arrow if we are in the navigating phase
  if (navigationState == NAVIGATING) {
    ImageType arrowImage = selectArrowImage(relativeDirection);
    displayImage(arrowImage);
  }

  // Start of the trail
  if (!trailStarted) {
    if (distance <= checkpointTrigger) {  // "Closeness" threshold
      trailStarted = true;
      currentStop = 1;  // Moving towards the first checkpoint
      Serial.println("Trail started. Heading to Stop 1.");
      navigationState = NAVIGATING;
    } else {
      Serial.println("Please proceed to the start of the trail.");
      if (dataReceived) {
        displayImage(GOTOSTART);  // Indicating to go to the starting point
      }
    }
  }
  // Navigating the trail
  else {
    if (distance <= checkpointTrigger && currentStop <= numberOfStops) {
      if (navigationState != AT_CHECKPOINT) {
        // Just arrived at this checkpoint
        Serial.print("Arrived at Stop ");
        Serial.println(currentStop);
        ImageType checkpointImage = static_cast<ImageType>(CHECKPOINT_1 + currentStop - 1);
        displayImage(checkpointImage);    // Show the checkpoint image
        navigationState = AT_CHECKPOINT;  // Update state to at checkpoint
        lastCheckpointTime = millis();    // Capture the time we arrived at the checkpoint

        // Check if this is the final stop
        if (currentStop == numberOfStops) {
          Serial.println("Final stop reached. Trail is complete.");
          // Display MY_PENDING image to indicate completion
          displayImage(MY_PENDING);
          // Optionally, you might want to change the navigation state or take other actions here
          navigationState = TRAIL_ENDED;  // Resetting the state to NOT_STARTED or another appropriate state
        }

        proximityVibrationTriggered = false;  // Allow vibration to trigger again for the next stop
        currentStop++;                        // Prepare for the next stop or complete the trail
      }
    } else if (navigationState == AT_CHECKPOINT) {
      if (millis() - lastCheckpointTime > 5000) {  // 5 seconds have passed since arriving at the checkpoint
        navigationState = NAVIGATING;              // Transition back to navigating after the delay
        proximityVibrationTriggered = false;       // Reset vibration trigger flag
      }
    } else if (navigationState == NAVIGATING && currentStop <= numberOfStops) {
      // Continue with the condition to update navigation info only if there's a significant change in distance
      if (abs(lastDistance - distance) > 0.5) {
        Serial.print("Distance to next stop: ");
        Serial.print(distance, 1);  // One decimal place for distance
        Serial.print(" meters. Direction to next stop: ");
        Serial.print(cardinal);
        Serial.print(" (Target angle: ");
        Serial.print(targetAngle);
        Serial.println(" degrees)");
        lastDistance = distance;  // Update lastDistance for next comparison

        // Here, potentially display the arrow again if needed, based on your logic for selecting and displaying arrows
        ImageType arrowImage = selectArrowImage(relativeDirection);
        displayImage(arrowImage);

        // Set the flag to start continuous vibration when within a certain distance from the next stop
        if (distance <= vibrationTrigger && !proximityVibrationTriggered) {
          proximityVibrationTriggered = true;
          lastVibrationTime = millis();  // Ensure we start timing from now
        }
      }
    }
  }
}

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

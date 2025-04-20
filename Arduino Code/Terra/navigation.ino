// navigation.ino - Core navigation logic and state management

// Thresholds for triggers in meters
const int checkpointTrigger = 10;  // Distance from the checkpoint that is considered an arrival
const int vibrationTrigger = 20;   // When to trigger vibration to indicate the checkpoint is getting close

const double startLat = 51.536286769987;
const double startLon = -0.12449799356353357;
const double stopLats[] = { 51.536862, 51.537587, 51.53831, 51.538896, 51.539512, 51.540152, 51.540788, 51.541382, 51.541977, 51.542588, 51.536286769987 };
const double stopLons[] = { -0.125599, -0.126087, -0.126477, -0.126926, -0.127397, -0.127758, -0.128196, -0.128654, -0.129082, -0.129531, -0.12449799356353357 };
const int numberOfStops = 11;

// Navigation State Enum moved to Terra.ino
// Navigation state variables moved to Terra.ino
unsigned long lastCheckpointTime = 0;           // Timestamp of when the last checkpoint was reached

// Main navigation logic function
void determineTrailStatusAndNavigate() {
  static double lastDistance = -1;

  // Obtain the target coordinates based on the trail's current status
  // Accessing global startLat/Lon and stopLats/Lons from config.ino
  // Accessing global currentStop from Terra.ino
  double targetLat = !trailStarted ? startLat : stopLats[currentStop - 1];
  double targetLon = !trailStarted ? startLon : stopLons[currentStop - 1];

  // Calculate the current distance and direction to the target
  // Accessing global distance from Terra.ino
  distance = getDistanceTo(targetLat, targetLon); // Update global distance
  // String cardinal = getCardinalTo(targetLat, targetLon); // Cardinal not currently used for display
  int targetAngle = getCourseTo(targetLat, targetLon);
  int currentAngle = readCompass(); // Call function from compass.ino

  // Calculate the relative direction for navigation
  int relativeDirection = calculateRelativeDirection(currentAngle, targetAngle);

  // --- State Machine ---
  if (navigationState == NOT_STARTED) {
    if (!dataReceived) { // Check global dataReceived flag from Terra.ino
      displayImage(MY_PENDING);  // Show MY_PENDING only when waiting for the first GPS data
    } else {
      Serial.println("Please proceed to the start of the trail.");
      displayImage(GOTOSTART);  // Now we're sure we've received data, show GOTOSTART
    }

    // Transition to navigating state once within close range to the start and data has been received
    if (dataReceived && distance <= checkpointTrigger) { // Access checkpointTrigger from config.ino
      trailStarted = true; // Update global trailStarted flag
      currentStop = 1;     // Update global currentStop
      Serial.println("Trail started. Heading to Stop 1.");
      navigationState = NAVIGATING; // Change state
    }
    return;  // Skip rest of the function logic when NOT_STARTED
  }

  // Only update navigation arrow if we are in the navigating phase
  if (navigationState == NAVIGATING) {
    ImageType arrowImage = selectArrowImage(relativeDirection);
    displayImage(arrowImage); // Call function from display.ino
  }

  // Start of the trail logic (already handled by NOT_STARTED state transition)
  // This block seems redundant with the NOT_STARTED logic above and might be removable
  /*
  if (!trailStarted) {
      // ... (This logic seems covered by the NOT_STARTED state)
  }
  */
  // Navigating the trail
  // else { // Assuming trailStarted is true if not NOT_STARTED
    if (distance <= checkpointTrigger && currentStop <= numberOfStops) { // Access numberOfStops from config.ino
      if (navigationState != AT_CHECKPOINT) {
        // Just arrived at this checkpoint
        Serial.print("Arrived at Stop ");
        Serial.println(currentStop);
        ImageType checkpointImage = static_cast<ImageType>(CHECKPOINT_1 + currentStop - 1); // Use enum from images.ino
        displayImage(checkpointImage);    // Show the checkpoint image
        navigationState = AT_CHECKPOINT;  // Update state to at checkpoint
        lastCheckpointTime = millis();    // Capture the time we arrived at the checkpoint

        // Check if this is the final stop
        if (currentStop == numberOfStops) {
          Serial.println("Final stop reached. Trail is complete.");
          displayImage(MY_PENDING); // Display completion image
          navigationState = TRAIL_ENDED; // Set final state
        } else {
           // Only increment stop if not the final one, prepare for AT_CHECKPOINT delay
        }

        proximityVibrationTriggered = false;  // Stop proximity vibration - Access flag from vibration.ino
        // currentStop++; // Increment happens after AT_CHECKPOINT delay
      }
    } else if (navigationState == AT_CHECKPOINT) {
      // Stay at checkpoint for 5 seconds
      if (millis() - lastCheckpointTime > 5000) {
          if (currentStop < numberOfStops) { // Check if there are more stops
              currentStop++; // Prepare for the next stop
              Serial.print("Proceeding to Stop "); Serial.println(currentStop);
              navigationState = NAVIGATING;              // Transition back to navigating after the delay
              proximityVibrationTriggered = false;       // Reset vibration trigger flag for next leg
              lastDistance = -1; // Force distance update print
          } else {
              // We already reached the end, stay in AT_CHECKPOINT or TRAIL_ENDED state
              // This case might already be covered by the arrival logic setting TRAIL_ENDED
              if (navigationState != TRAIL_ENDED) {
                  Serial.println("Checkpoint delay ended, trail finished.");
                  navigationState = TRAIL_ENDED; // Ensure state is TRAIL_ENDED
              }
          }
      }
    } else if (navigationState == NAVIGATING && currentStop <= numberOfStops) {
      // Continue navigating towards the currentStop
      // Print distance update if changed significantly
      if (abs(lastDistance - distance) > 0.5) { // Print if distance changes by > 0.5m
        Serial.print("Distance to next stop ("); Serial.print(currentStop); Serial.print("): ");
        Serial.print(distance, 1);  // One decimal place for distance
        Serial.print(" m. Relative Direction: "); Serial.print(relativeDirection); Serial.println(" deg.");
        // Serial.print(" meters. Direction to next stop: "); Serial.print(cardinal); // Cardinal not displayed
        // Serial.print(" (Target angle: "); Serial.print(targetAngle); Serial.println(" degrees)");
        lastDistance = distance;  // Update lastDistance for next comparison

        // Arrow is updated at the start of NAVIGATING state block

        // Set the flag to start continuous vibration when within vibrationTrigger distance
        if (distance <= vibrationTrigger && !proximityVibrationTriggered) { // Access vibrationTrigger from config.ino
          Serial.println("Vibration proximity triggered.");
          proximityVibrationTriggered = true; // Access flag from vibration.ino
          lastVibrationTime = millis();  // Ensure we start timing from now - Access var from vibration.ino
          triggerProximityVibration(); // Trigger first vibration immediately - Call func from vibration.ino
        } else if (distance > vibrationTrigger && proximityVibrationTriggered) {
            Serial.println("Exited vibration proximity.");
            proximityVibrationTriggered = false; // Stop vibration if moved away
        }
      }
    } else if (navigationState == TRAIL_ENDED) {
        // Do nothing further, trail is complete.
    }
  // } // End of 'else' assuming trailStarted
}


// --- Navigation Helper Functions ---

// Calculate relative direction based on angles
int calculateRelativeDirection(int currentAngle, int targetAngle) {
  int difference = targetAngle - currentAngle;
  // Normalize difference to -180..+180
  while (difference <= -180) difference += 360;
  while (difference > 180) difference -= 360;

  // Convert to 0..359 for arrow selection
  int relativeAngle = difference;
   if (relativeAngle < 0) {
       relativeAngle += 360;
   }
  return relativeAngle;
}


// Select an arrow image based on relative direction (0-359 degrees)
ImageType selectArrowImage(int relativeDirection) {
  // Normalize angle just in case (already done by calculateRelativeDirection)
  // relativeDirection = (relativeDirection % 360 + 360) % 360;

  if (relativeDirection >= 348.75 || relativeDirection < 11.25) return ARROW_N;
  if (relativeDirection >= 11.25 && relativeDirection < 33.75) return ARROW_NNE;
  if (relativeDirection >= 33.75 && relativeDirection < 56.25) return ARROW_NE;
  if (relativeDirection >= 56.25 && relativeDirection < 78.75) return ARROW_ENE;
  if (relativeDirection >= 78.75 && relativeDirection < 101.25) return ARROW_E;
  if (relativeDirection >= 101.25 && relativeDirection < 123.75) return ARROW_ESE;
  if (relativeDirection >= 123.75 && relativeDirection < 146.25) return ARROW_SE;
  if (relativeDirection >= 146.25 && relativeDirection < 168.75) return ARROW_SSE;
  if (relativeDirection >= 168.75 && relativeDirection < 191.25) return ARROW_S;
  if (relativeDirection >= 191.25 && relativeDirection < 213.75) return ARROW_SSW;
  if (relativeDirection >= 213.75 && relativeDirection < 236.25) return ARROW_SW;
  if (relativeDirection >= 236.25 && relativeDirection < 258.75) return ARROW_WSW;
  if (relativeDirection >= 258.75 && relativeDirection < 281.25) return ARROW_W;
  if (relativeDirection >= 281.25 && relativeDirection < 303.75) return ARROW_WNW;
  if (relativeDirection >= 303.75 && relativeDirection < 326.25) return ARROW_NW;
  if (relativeDirection >= 326.25 && relativeDirection < 348.75) return ARROW_NNW;

  return ARROW_N; // Default fallback
}

// Simple function to get distance to lat/lon, uses global currentLat/Lon
double getDistanceTo(double lat, double lon) {
  // Accessing global currentLat, currentLon from Terra.ino
  return TinyGPSPlus::distanceBetween(currentLat, currentLon, lat, lon);
}

// Get cardinal direction (string) - Not currently used for display
String getCardinalTo(double lat, double lon) {
  // Accessing global currentLat, currentLon from Terra.ino
  double courseTo = TinyGPSPlus::courseTo(currentLat, currentLon, lat, lon);
  String cardinal = TinyGPSPlus::cardinal(courseTo);
  return cardinal;
}

// Get course/bearing (integer degrees) to target lat/lon
int getCourseTo(double lat, double lon) {
  // Accessing global currentLat, currentLon from Terra.ino
  return TinyGPSPlus::courseTo(currentLat, currentLon, lat, lon);
}

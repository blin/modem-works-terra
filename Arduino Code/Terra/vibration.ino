// vibration.ino - Handles the DRV2605 vibration motor

// Vibration motor object
Adafruit_DRV2605 drv;

// Vibration settings and state
int effectNumber = 58; // Default vibration effect
bool proximityVibrationTriggered = false; // Flag if vibration is active due to proximity
unsigned long lastVibrationTime = 0;   // Tracks the last time vibration was triggered for continuous pulsing

// Trigger vibration based on proximity rules
void triggerProximityVibration() {
  // Check if we should still be vibrating (if within a certain distance and navigating)
  // Note: 'distance' and 'navigationState' are global variables accessed from Terra.ino/navigation.ino
  if (proximityVibrationTriggered && navigationState == NAVIGATING) {
    // set the effect to play
    drv.setWaveform(0, effectNumber);  // play effect
    drv.setWaveform(1, 0);             // end waveform
    // play the effect!
    drv.go();
    // Serial.println("Vibration triggered"); // Optional debug
    // Note: We do not set proximityVibrationTriggered to false here for continuous vibration logic in loop()
  } else {
    // Ensure vibration stops if conditions are no longer met
    proximityVibrationTriggered = false;
  }
}

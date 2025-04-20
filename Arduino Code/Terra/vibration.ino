// vibration.ino - Handles the DRV2605 vibration motor

// Vibration motor object declaration moved to Terra.ino
// Vibration state variables moved to Terra.ino
// https://cdn-shop.adafruit.com/datasheets/DRV2605.pdf - Waveform Library Effects List
int effectNumber = 58;

// Trigger vibration based on proximity rules
void triggerProximityVibration() {
  // Check if we should still be vibrating (if within a certain distance and navigating)
  // Note: 'distance' and 'navigationState' are global variables accessed from Terra.ino/navigation.ino
  if (proximityVibrationTriggered) {
    // set the effect to play
    drv.setWaveform(0, effectNumber);  // play effect
    drv.setWaveform(1, 0);             // end waveform
    // play the effect!
    drv.go();
    // Serial.println("Vibration triggered"); // Optional debug
    // Note: We do not set proximityVibrationTriggered to false here for continuous vibration logic in loop()
    proximityVibrationTriggered = false;
  }
}

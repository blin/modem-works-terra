// gps.ino - Handles GPS communication and data parsing

// Checks if the GPS object has a valid location fix.
bool locationKnown(TinyGPSPlus& gps) {
  return gps.location.isValid();
}

// Updates global latitude and longitude variables if the location is known.
// Note: Still relies on global currentLat, currentLon, dataReceived.
// Consider passing these as references too for better encapsulation.
void updateLocationGlobals(TinyGPSPlus& gps) {
  if (locationKnown(gps)) { // Call the updated locationKnown
    currentLat = gps.location.lat();
    currentLon = gps.location.lng();
    dataReceived = true; // Mark that we have received valid data
  }
}

// Reads data from Serial1 and feeds it to the TinyGPSPlus object for a specified duration.
static void feedGPSObject(TinyGPSPlus& gps, unsigned long ms) {
  unsigned long start = millis();
  do {
    // While data is available from the GPS module connected to Serial1
    while (Serial1.available()) {
      // Feed the incoming character to the TinyGPSPlus object for parsing
      gps.encode(Serial1.read());
    }
    // Continue processing until the specified time (ms) has elapsed
  } while (millis() - start < ms);
}

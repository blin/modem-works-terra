// gps.ino - Handles GPS communication and data parsing

// Checks if the GPS object has a valid location fix.
bool locationKnown(TinyGPSPlus& gps) {
  // gps.location is the object storing lat/lon data within TinyGPSPlus.
  // .isValid() is a method of the location object that returns true if
  // the library has successfully parsed valid location data from the GPS module.
  return gps.location.isValid();
}

/**
 * @brief Updates latitude, longitude, and a data received flag based on the GPS object's state.
 *        If the location is valid, the provided variables are updated with the current
 *        latitude, longitude, and the received flag is set to true.
 *
 * @param gps A reference to the TinyGPSPlus object to read location data from.
 * @param lat A reference (read-write) to the variable where the latitude should be stored.
 * @param lon A reference (read-write) to the variable where the longitude should be stored.
 * @param received A reference (read-write) to a boolean flag that will be set to true
 *                 if valid data is received and the location is updated.
 */
void updateLocationGlobals(TinyGPSPlus& gps, double& lat, double& lon, bool& received) {
  // Check if the GPS object has a valid location fix using the locationKnown function
  if (locationKnown(gps)) {
    // Update the passed-in variables (via references) with the new coordinates
    lat = gps.location.lat();
    lon = gps.location.lng();
    // Set the received flag to true since we successfully updated lat and lon
    received = true;
  }
  // If locationKnown(gps) is false, the function does nothing, leaving lat, lon,
  // and received unchanged. The caller is responsible for handling the case
  // where location is not known or data wasn't updated.
}

/**
 * @brief Reads data from a specified HardwareSerial port and feeds it to the TinyGPSPlus object.
 *        This function will read from the serial port for a given duration (ms).
 *
 * @param gps A reference to the TinyGPSPlus object to feed data into.
 * @param gpsStream A reference to the HardwareSerial port connected to the GPS module (e.g., Serial1).
 * @param ms The duration in milliseconds to read data from the serial port.
 */
static void processGPSStream(TinyGPSPlus& gps, HardwareSerial& gpsStream, unsigned long ms) {
  unsigned long start = millis();
  do {
    // While data is available from the specified GPS serial stream
    while (gpsStream.available()) {
      // Feed the incoming character to the TinyGPSPlus object for parsing
      gps.encode(gpsStream.read());
    }
    // Continue processing until the specified time (ms) has elapsed
  } while (millis() - start < ms);
}

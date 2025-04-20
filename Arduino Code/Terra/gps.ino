// gps.ino - Handles GPS communication and data parsing

// The TinyGPSPlus object
TinyGPSPlus gps;

// Handle GPS Data
void handleGPSData() {
  if (debugMode) {  // If we're in debug mode, read the serial input
    if (Serial.available()) {
      if (readSerialGPS()) {
        dataReceived = true;  // Once the first data has arrived, we can get into our start routine
      }
    }
  } else {  // If we're not in serial input mode, start handling real GPS data
    while (Serial1.available()) {
      if (readGPS()) {
        dataReceived = true;
      }
      // smart delay not only delays, but also processes data from
      // the GPS device over the serial port...
      smartDelay(5000);
    }
  }
}

// Read the GPS signal for current lat/lon - this is a serial input for now but can be replaced with code to read the GPS module
bool readSerialGPS() {
  String inputString = Serial.readStringUntil('\n');
  int commaIndex = inputString.indexOf(',');

  // Check if the comma exists and it's not at the end of the string
  if (commaIndex != -1 && commaIndex < inputString.length() - 1) {
    currentLat = inputString.substring(0, commaIndex).toDouble();
    currentLon = inputString.substring(commaIndex + 1).toDouble();
    return true;  // Data was successfully parsed
  } else {
    Serial.println("Invalid format. Please enter in format: lat,lon");
    return false;  // Data parsing failed
  }
}

// Read the GPS module signal for current lat/lon
bool readGPS() {
  if (gps.location.isValid()) {
    currentLat = gps.location.lat();
    currentLon = gps.location.lng();
    Serial.printf("lat=%f,lon=%f\n", currentLat, currentLon);

    return true;
  }
  Serial.println("GPS location is invalid!");

  return false;
}

// This custom version of delay() ensures that the gps object is being "fed".
static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
  Serial.printf("gps sat=%d,sentencesWithFix=%d\n", gps.satellites.value(), gps.sentencesWithFix());
}

// gps.ino - Handles GPS communication and data parsing

bool locationKnown() {
  return gps.location.isValid();
}

void updateLocationGlobals() {
  if (locationKnown()) {
    currentLat = gps.location.lat();
    currentLon = gps.location.lng();

    dataReceived = true;
  }
}

static void feedGPSObject(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}

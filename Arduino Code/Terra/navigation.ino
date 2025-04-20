void handleNotStartedState() {
  drawText(tft, Serial, "Location is known\nlat=%.3f\nlon=%.3f\n", currentLat, currentLon);
  delay(2000);
  int ourAngle = readCompass();
  drawText(tft, Serial, "angle=%d\n", ourAngle);
  delay(2000);

  targetLat = currentLat + 0.00180;
  targetLon = currentLon;
  drawText(tft, Serial, "Target is known\nlat=%.3f\nlon=%.3f\n", targetLat, targetLon);
  delay(2000);
}

// Handle state when actively navigating between checkpoints
void handleNavigatingState() {
  distance = getDistanceTo(targetLat, targetLon); // Update global distance
  // String cardinal = getCardinalTo(targetLat, targetLon); // Cardinal not currently used for display
  int targetAngle = getCourseTo(targetLat, targetLon);
  int currentAngle = readCompass(); // Call function from compass.ino

  // Calculate the relative direction for navigation
  int relativeDirection = calculateRelativeDirection(currentAngle, targetAngle);

  drawText(tft, Serial, "Distance between\nlat=%.3f\nlon=%.3f\nand\nlat=%.3f\nlon=%.3f\n", currentLat, currentLon, targetLat, targetLon);
  delay(2000);


  drawText(tft, Serial, "%.1f meters left\n", distance);
  delay(2000);
  drawText(tft, Serial, "reldir=%d\n", relativeDirection);
  delay(2000);

  // Update navigation arrow
  ImageType arrowImage = selectArrowImage(relativeDirection);
  displayImage(arrowImage);
  delay(2000);
}

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

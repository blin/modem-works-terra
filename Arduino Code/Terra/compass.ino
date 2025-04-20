// compass.ino - Handles reading the CMPS12 compass sensor

// Variables used by readCompass
unsigned char high_byte, low_byte, angle8;
char pitch, roll;
unsigned int angle16;

// Read the compass sensor and return the compassDirection in degrees (0-359)
int readCompass() {
  Wire.beginTransmission(CMPS12_ADDRESS);  // Starts communication with CMPS12
  Wire.write(ANGLE_8);                     // Sends the register we wish to start reading from
  Wire.endTransmission();

  // Request 5 bytes from the CMPS12
  // this will give us the 8 bit bearing,
  // both bytes of the 16 bit bearing, pitch and roll
  Wire.requestFrom(CMPS12_ADDRESS, 5);

  while (Wire.available() < 5)
    ;  // Wait for all bytes to come back

  angle8 = Wire.read();  // Read back the 5 bytes
  high_byte = Wire.read();
  low_byte = Wire.read();
  pitch = Wire.read();
  roll = Wire.read();

  angle16 = high_byte;  // Calculate 16 bit angle
  angle16 <<= 8;
  angle16 += low_byte;

  int currentAngle = angle16 / 10; // The angle is in tenths of a degree, convert to degrees
  return currentAngle;
}

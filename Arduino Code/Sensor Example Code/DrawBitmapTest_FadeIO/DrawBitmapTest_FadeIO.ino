/* Working example to demonstrate the drawing of X BitMap (XBM) format image onto the TFT display.
Information on the X BitMap (XBM) format can be found here: https://en.wikipedia.org/wiki/X_BitMap
This is a modified code example from the TFT_eSPI library, created by Bodmer 23/04/18
Last edit: 23 Feb 2024.

This code displays an image, fades the screen up and down again before switching to a new image.

Guide to configuring images:
1. Resize your image to 240px x 240px
2. Convert to greyscale
3. Export as .bmp using a software like GIMP
4. Convert .bmp to XBM using https://www.online-utility.org/image/convert/to/XBM
5. Open XBM files in a code or text editor and copy the array (starts with 0xFF)
6. Paste the array into the template .h file, eg. "PROGMEM const unsigned char checkpoint_1[] = {}"
7. Save into a .h file within the sketch folder, eg. "checkpoint_1.h"

Make sure the names of the files are consistent across all of the code files.

*/

#include "checkpoint_1.h"    // Sketch tab header for xbm images
#include "checkpoint_2.h"    // Sketch tab header for xbm images
#include "checkpoint_3.h"    // Sketch tab header for xbm images
#include "checkpoint_4.h"    // Sketch tab header for xbm images
#include "checkpoint_5.h"    // Sketch tab header for xbm images

#include "Adafruit_GC9A01A.h"

#define TFT_DC 33
#define TFT_CS 15
#define TFT_BL 12

#define logoWidth  240
#define logoHeight 240

// NOTE: the images are "inverted", so the whole screen is filled
// with FG_COLOR, and then the "inverted" image is drawn with BG_COLOR.
// tft.fillScreen(FG_COLOR);
// tft.drawXBitmap(x, y, checkpoint_4, logoWidth, logoHeight, BG_COLOR);
#define BG_COLOR GC9A01A_BLACK
#define FG_COLOR GC9A01A_WHITE

Adafruit_GC9A01A tft(TFT_CS, TFT_DC);


// Set X and Y coordinates where the image will be drawn
int x = 0;
int y = 0;

void setup() {
  pinMode(TFT_BL, OUTPUT);
  tft.begin();
  tft.fillScreen(FG_COLOR);
}

void loop() {
  fadeOut();
  tft.fillScreen(FG_COLOR);
  tft.drawXBitmap(x, y, checkpoint_1, logoWidth, logoHeight, BG_COLOR);
  delay(100);
  fadeIn();

  delay(1000);

  fadeOut();
  tft.fillScreen(FG_COLOR);
  tft.drawXBitmap(x, y, checkpoint_2, logoWidth, logoHeight, BG_COLOR);
  delay(100);
  fadeIn();

  delay(1000);

  fadeOut();
  tft.fillScreen(FG_COLOR);
  tft.drawXBitmap(x, y, checkpoint_3, logoWidth, logoHeight, BG_COLOR);
  delay(100);
  fadeIn();

  delay(1000);

  fadeOut();
  tft.fillScreen(FG_COLOR);
  tft.drawXBitmap(x, y, checkpoint_4, logoWidth, logoHeight, BG_COLOR);
  delay(100);
  fadeIn();

  delay(1000);
}

void fadeOut() {
  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
    // sets the value (range from 0 to 255):
    analogWrite(TFT_BL, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(10);
  }
}

void fadeIn(){
  // fade in from min to max in increments of 5 points:
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    analogWrite(TFT_BL, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(10);
  }
}

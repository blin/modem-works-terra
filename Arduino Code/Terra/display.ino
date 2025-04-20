// display.ino - Handles TFT display functions

// Include image headers here so they are available to displayImage
#include "images/pending.h"
#include "images/gotostart.h"
#include "images/arrows/arrow_N.h"
#include "images/arrows/arrow_NNE.h"
#include "images/arrows/arrow_NE.h"
#include "images/arrows/arrow_ENE.h"
#include "images/arrows/arrow_E.h"
#include "images/arrows/arrow_ESE.h"
#include "images/arrows/arrow_SE.h"
#include "images/arrows/arrow_SSE.h"
#include "images/arrows/arrow_S.h"
#include "images/arrows/arrow_SSW.h"
#include "images/arrows/arrow_SW.h"
#include "images/arrows/arrow_WSW.h"
#include "images/arrows/arrow_W.h"
#include "images/arrows/arrow_WNW.h"
#include "images/arrows/arrow_NW.h"
#include "images/arrows/arrow_NNW.h"
#include "images/checkpoints/checkpoint_1.h"
#include "images/checkpoints/checkpoint_2.h"
#include "images/checkpoints/checkpoint_3.h"
#include "images/checkpoints/checkpoint_4.h"
#include "images/checkpoints/checkpoint_5.h"
#include "images/checkpoints/checkpoint_6.h"
#include "images/checkpoints/checkpoint_7.h"
#include "images/checkpoints/checkpoint_8.h"
#include "images/checkpoints/checkpoint_9.h"
#include "images/checkpoints/checkpoint_10.h"

#define logoWidth  240
#define logoHeight 240
#define BG_COLOR GC9A01A_BLACK
#define FG_COLOR GC9A01A_WHITE
ImageType currentDisplayedImage = NONE; // Tracks the currently displayed image

// Display image function - fades images out then in again
void displayImage(ImageType image) {
  if (currentDisplayedImage != image) {
    if (screenOn) {
      fadeOut();
    }
    tft.fillScreen(FG_COLOR);
    switch (image) {
      case MY_PENDING:
        drawBitmap(pending);
        Serial.println("Drawn pending.h");
        break;
      case GOTOSTART:
        drawBitmap(gotostart);
        Serial.println("Drawn gotostart.h");
        break;
      case ARROW_N:
        drawBitmap(arrow_N);
        Serial.println("Drawn arrow_N.h");
        break;
      case ARROW_NNE:
        drawBitmap(arrow_NNE);
        Serial.println("Drawn arrow_NNE.h");
        break;
      case ARROW_NE:
        drawBitmap(arrow_NE);
        Serial.println("Drawn arrow_NE.h");
        break;
      case ARROW_ENE:
        drawBitmap(arrow_ENE);
        Serial.println("Drawn arrow_ENE.h");
        break;
      case ARROW_E:
        drawBitmap(arrow_E);
        Serial.println("Drawn arrow_E.h");
        break;
      case ARROW_ESE:
        drawBitmap(arrow_ESE);
        Serial.println("Drawn arrow_ESE.h");
        break;
      case ARROW_SE:
        drawBitmap(arrow_SE);
        Serial.println("Drawn arrow_SE.h");
        break;
      case ARROW_SSE:
        drawBitmap(arrow_SSE);
        Serial.println("Drawn arrow_SSE.h");
        break;
      case ARROW_S:
        drawBitmap(arrow_S);
        Serial.println("Drawn arrow_S.h");
        break;
      case ARROW_SSW:
        drawBitmap(arrow_SSW);
        Serial.println("Drawn arrow_SSW.h");
        break;
      case ARROW_SW:
        drawBitmap(arrow_SW);
        Serial.println("Drawn arrow_SW.h");
        break;
      case ARROW_WSW:
        drawBitmap(arrow_WSW);
        Serial.println("Drawn arrow_WSW.h");
        break;
      case ARROW_W:
        drawBitmap(arrow_W);
        Serial.println("Drawn arrow_W.h");
        break;
      case ARROW_WNW:
        drawBitmap(arrow_WNW);
        Serial.println("Drawn arrow_WNW.h");
        break;
      case ARROW_NW:
        drawBitmap(arrow_NW);
        Serial.println("Drawn arrow_NW.h");
        break;
      case ARROW_NNW:
        drawBitmap(arrow_NNW);
        Serial.println("Drawn arrow_NNW.h");
        break;
      case CHECKPOINT_1:
        drawBitmap(checkpoint_1);
        Serial.println("Drawn checkpoint_1.h");
        break;
      case CHECKPOINT_2:
        drawBitmap(checkpoint_2);
        Serial.println("Drawn checkpoint_2.h");
        break;
      case CHECKPOINT_3:
        drawBitmap(checkpoint_3);
        Serial.println("Drawn checkpoint_3.h");
        break;
      case CHECKPOINT_4:
        drawBitmap(checkpoint_4);
        Serial.println("Drawn checkpoint_4.h");
        break;
      case CHECKPOINT_5:
        drawBitmap(checkpoint_5);
        Serial.println("Drawn checkpoint_5.h");
        break;
      case CHECKPOINT_6:
        drawBitmap(checkpoint_6);
        Serial.println("Drawn checkpoint_6.h");
        break;
      case CHECKPOINT_7:
        drawBitmap(checkpoint_7);
        Serial.println("Drawn checkpoint_7.h");
        break;
      case CHECKPOINT_8:
        drawBitmap(checkpoint_8);
        Serial.println("Drawn checkpoint_8.h");
        break;
      case CHECKPOINT_9:
        drawBitmap(checkpoint_9);
        Serial.println("Drawn checkpoint_9.h");
        break;
      case CHECKPOINT_10:
        drawBitmap(checkpoint_10);
        Serial.println("Drawn checkpoint_10.h");
        break;
      default:
        tft.fillScreen(FG_COLOR);
        Serial.println("No image to display");
    }
    fadeIn();
    currentDisplayedImage = image;
  }
}

void drawBitmap(const unsigned char* bitmap) {
  tft.drawXBitmap(0, 0, bitmap, logoWidth, logoHeight, BG_COLOR);
}

void fadeOut() {
  for (int i = 255; i >= 0; i -= 5) {
    analogWrite(GC9A01A_BL, i);
    delay(10);
  }
  screenOn = false;
}

void fadeIn() {
  for (int i = 0; i <= 255; i += 5) {
    analogWrite(GC9A01A_BL, i);
    delay(10);
  }
  screenOn = true;
}

/**
 * @brief Draws formatted text onto the specified display object.
 *
 * @param display A reference to the Adafruit_GFX compatible display object (e.g., tft).
 * @param format A printf-style format string.
 * @param ... Variable arguments for the format string.
 * @return size_t The number of characters printed.
 */
size_t drawText(Adafruit_GFX& display, const char *format, ...) {
  // Note: Using Adafruit_GFX allows flexibility but assumes methods like fillScreen,
  // setCursor, setTextColor, setTextSize, print are available.
  // Specific color constants like GC9A01A_BLACK might need adjustment if using a different display class.
  display.fillScreen(GC9A01A_BLACK); // Assuming BLACK is a common color define or use a generic one if available
  display.setCursor(0, 100);
  display.setTextColor(GC9A01A_GREEN); // Assuming GREEN is common or use a generic one
  display.setTextSize(2);

  va_list args;
  va_start(args, format);
  char buffer[64]; // Adjust size as needed, consider making it larger for safety
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);

  // Use the passed display object to print
  return display.print(buffer);
}

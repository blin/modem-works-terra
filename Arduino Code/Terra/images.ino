// images.ino - Includes image headers and defines image types

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

// Image Types Enum
enum ImageType {
  NONE,
  MY_PENDING,
  GOTOSTART,
  ARROW_N,
  ARROW_NNE,
  ARROW_NE,
  ARROW_ENE,
  ARROW_E,
  ARROW_ESE,
  ARROW_SE,
  ARROW_SSE,
  ARROW_S,
  ARROW_SSW,
  ARROW_SW,
  ARROW_WSW,
  ARROW_W,
  ARROW_WNW,
  ARROW_NW,
  ARROW_NNW,
  CHECKPOINT_1,
  CHECKPOINT_2,
  CHECKPOINT_3,
  CHECKPOINT_4,
  CHECKPOINT_5,
  CHECKPOINT_6,
  CHECKPOINT_7,
  CHECKPOINT_8,
  CHECKPOINT_9,
  CHECKPOINT_10
};

ImageType currentDisplayedImage = NONE; // Tracks the currently displayed image

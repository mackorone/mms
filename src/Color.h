#pragma once

#include <QChar>
#include <QMap>

#include "RGB.h"

namespace mms {

enum class Color {
  BLACK,
  BLUE,
  CYAN,
  GRAY,
  GREEN,
  ORANGE,
  RED,
  WHITE,
  YELLOW,
  DARK_BLUE,
  DARK_CYAN,
  DARK_GRAY,
  DARK_GREEN,
  DARK_ORANGE,
  DARK_RED,
  DARK_VIOLET,
  DARK_YELLOW,
};

const QMap<QChar, Color> &CHAR_TO_COLOR();
const QMap<Color, RGB> &COLOR_TO_RGB();

}  // namespace mms

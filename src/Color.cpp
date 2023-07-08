#include "Color.h"

namespace mms {

const QMap<QChar, Color> &CHAR_TO_COLOR() {
  static const QMap<QChar, Color> map = {
      {'k', Color::BLACK},       {'b', Color::BLUE},
      {'a', Color::GRAY},        {'c', Color::CYAN},
      {'g', Color::GREEN},       {'o', Color::ORANGE},
      {'r', Color::RED},         {'w', Color::WHITE},
      {'y', Color::YELLOW},      {'B', Color::DARK_BLUE},
      {'C', Color::DARK_CYAN},   {'A', Color::DARK_GRAY},
      {'G', Color::DARK_GREEN},  {'O', Color::DARK_ORANGE},
      {'R', Color::DARK_RED},    {'V', Color::DARK_VIOLET},
      {'Y', Color::DARK_YELLOW},
  };
  return map;
}

const QMap<Color, RGB> &COLOR_TO_RGB() {
  static const QMap<Color, RGB> map = {
      {Color::BLACK, {0, 0, 0}},         {Color::BLUE, {0, 0, 179}},
      {Color::CYAN, {0, 102, 102}},      {Color::GRAY, {179, 179, 179}},
      {Color::GREEN, {0, 179, 0}},       {Color::ORANGE, {179, 102, 0}},
      {Color::RED, {204, 0, 0}},         {Color::WHITE, {255, 255, 255}},
      {Color::YELLOW, {179, 179, 0}},    {Color::DARK_BLUE, {0, 0, 51}},
      {Color::DARK_CYAN, {0, 51, 51}},   {Color::DARK_GRAY, {26, 26, 26}},
      {Color::DARK_GREEN, {0, 77, 0}},   {Color::DARK_ORANGE, {51, 26, 0}},
      {Color::DARK_RED, {77, 0, 0}},     {Color::DARK_VIOLET, {51, 0, 51}},
      {Color::DARK_YELLOW, {51, 51, 0}},
  };
  return map;
}

}  // namespace mms

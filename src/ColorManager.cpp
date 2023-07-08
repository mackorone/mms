#include "ColorManager.h"

#include "AssertMacros.h"
#include "Color.h"
#include "Settings.h"

namespace mms {

const QString ColorManager::GROUP = "colors";
const QString ColorManager::KEY_TILE_BASE_COLOR = "mouse-base-color";
const QString ColorManager::KEY_TILE_WALL_COLOR = "mouse-wall-color";
const QString ColorManager::KEY_TILE_CORNER_COLOR = "mouse-corner-color";
const QString ColorManager::KEY_MOUSE_BODY_COLOR = "mouse-body-color";
const QString ColorManager::KEY_MOUSE_WHEEL_COLOR = "mouse-wheel-color";
const QString ColorManager::KEY_TILE_WALL_IS_SET_COLOR =
    "tile-wall-is-set-color";
const QString ColorManager::KEY_TILE_WALL_NOT_SET_ALPHA =
    "tile-wall-not-set-alpha";

ColorManager *ColorManager::INSTANCE = nullptr;

void ColorManager::init() {
  ASSERT_TR(INSTANCE == nullptr);
  INSTANCE = new ColorManager();
}

ColorManager *ColorManager::get() {
  ASSERT_FA(INSTANCE == nullptr);
  return INSTANCE;
}

Color ColorManager::getTileBaseColor() { return m_tileBaseColor; }

Color ColorManager::getTileWallColor() { return m_tileWallColor; }

Color ColorManager::getTileCornerColor() { return m_tileCornerColor; }

Color ColorManager::getMouseBodyColor() { return m_mouseBodyColor; }

Color ColorManager::getMouseWheelColor() { return m_mouseWheelColor; }

Color ColorManager::getTileWallIsSetColor() { return m_tileWallIsSetColor; }

unsigned char ColorManager::getTileWallNotSetAlpha() {
  return m_tileWallNotSetAlpha;
}

void ColorManager::update(Color tileBaseColor, Color tileWallColor,
                          Color mouseBodyColor, Color mouseWheelColor,
                          Color tileWallIsSetColor,
                          unsigned char tileWallNotSetAlpha) {
  m_tileBaseColor = tileBaseColor;
  m_tileWallColor = tileWallColor;
  m_mouseBodyColor = mouseBodyColor;
  m_mouseWheelColor = mouseWheelColor;
  m_tileWallIsSetColor = tileWallIsSetColor;
  m_tileWallNotSetAlpha = tileWallNotSetAlpha;

  setValue(KEY_TILE_BASE_COLOR, CHAR_TO_COLOR().key(tileBaseColor));
  setValue(KEY_TILE_WALL_COLOR, CHAR_TO_COLOR().key(tileWallColor));
  setValue(KEY_MOUSE_BODY_COLOR, CHAR_TO_COLOR().key(mouseBodyColor));
  setValue(KEY_MOUSE_WHEEL_COLOR, CHAR_TO_COLOR().key(mouseWheelColor));
  setValue(KEY_TILE_WALL_IS_SET_COLOR, CHAR_TO_COLOR().key(tileWallIsSetColor));
  setValue(KEY_TILE_WALL_NOT_SET_ALPHA, QString::number(tileWallNotSetAlpha));
}

ColorManager::ColorManager()
    : m_tileBaseColor(getColor(KEY_TILE_BASE_COLOR, Color::BLACK)),
      m_tileWallColor(getColor(KEY_TILE_WALL_COLOR, Color::RED)),
      m_tileCornerColor(getColor(KEY_TILE_CORNER_COLOR, Color::GRAY)),
      m_mouseBodyColor(getColor(KEY_MOUSE_BODY_COLOR, Color::BLUE)),
      m_mouseWheelColor(getColor(KEY_MOUSE_WHEEL_COLOR, Color::GREEN)),
      m_tileWallIsSetColor(getColor(KEY_TILE_WALL_IS_SET_COLOR, Color::RED)),
      m_tileWallNotSetAlpha(getNumber(KEY_TILE_WALL_NOT_SET_ALPHA, 64)) {}

Color ColorManager::getColor(QString key, Color defaultValue) {
  QString string = Settings::get()->value(GROUP, key);
  if (string.isEmpty() || !CHAR_TO_COLOR().contains(string.at(0))) {
    setValue(key, CHAR_TO_COLOR().key(defaultValue));
    return defaultValue;
  }
  return CHAR_TO_COLOR().value(string.at(0));
}

unsigned char ColorManager::getNumber(QString key, unsigned char defaultValue) {
  bool ok = true;
  QString string = Settings::get()->value(GROUP, key);
  unsigned char number = string.toUShort(&ok);
  if (!ok) {
    number = defaultValue;
    setValue(key, QString::number(number));
  }
  return number;
}

void ColorManager::setValue(QString key, QString value) {
  Settings::get()->update(GROUP, key, value);
}

}  // namespace mms

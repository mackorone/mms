#include "ColorManager.h"

#include "Assert.h"
#include "Settings.h"
#include "SimUtilities.h"

namespace mms {

const QString ColorManager::GROUP = "colors";
const QString ColorManager::KEY_TILE_BASE_COLOR = "tile-base-color";
const QString ColorManager::KEY_TILE_WALL_COLOR = "tile-wall-color";
const QString ColorManager::KEY_TILE_CORNER_COLOR = "tile-corner-color";
const QString ColorManager::KEY_UNDECLARED_WALL_COLOR = 
    "undeclared-wall-color";
const QString ColorManager::KEY_UNDECLARED_NO_WALL_COLOR =
    "undeclared-no-wall-color";
const QString ColorManager::KEY_INCORRECTLY_DECLARED_WALL_COLOR =
    "incorrectly-declared-wall-color";
const QString ColorManager::KEY_INCORRECTLY_DECLARED_NO_WALL_COLOR =
    "incorrectly-declared-no-wall-color";
const QString ColorManager::KEY_MOUSE_BODY_COLOR = "mouse-body-color";
const QString ColorManager::KEY_MOUSE_WHEEL_COLOR = "mouse-wheel-color";

ColorManager* ColorManager::INSTANCE = nullptr;

void ColorManager::init() {
    ASSERT_TR(INSTANCE == nullptr);
    INSTANCE = new ColorManager();
}

ColorManager* ColorManager::get() {
    ASSERT_FA(INSTANCE == nullptr);
    return INSTANCE;
}

Color ColorManager::getTileBaseColor() {
    return m_tileBaseColor;
}

Color ColorManager::getTileWallColor() {
    return m_tileWallColor;
}

Color ColorManager::getTileCornerColor() {
    return m_tileCornerColor;
}

Color ColorManager::getUndeclaredWallColor() {
    return m_undeclaredWallColor;
}

Color ColorManager::getUndeclaredNoWallColor() {
    return m_undeclaredNoWallColor;
}

Color ColorManager::getIncorrectlyDeclaredWallColor() {
    return m_incorrectlyDeclaredWallColor;
}

Color ColorManager::getIncorrectlyDeclaredNoWallColor() {
    return m_incorrectlyDeclaredNoWallColor;
}

Color ColorManager::getMouseBodyColor() {
    return m_mouseBodyColor;
}

Color ColorManager::getMouseWheelColor() {
    return m_mouseWheelColor;
}

ColorManager::ColorManager() :
    m_tileBaseColor(getColor(
        KEY_TILE_BASE_COLOR,
        Color::BLACK
    )),
    m_tileWallColor(getColor(
        KEY_TILE_WALL_COLOR,
        Color::RED
    )),
    m_tileCornerColor(getColor(
        KEY_TILE_CORNER_COLOR,
        Color::GRAY
    )),
    m_undeclaredWallColor(getColor(
        KEY_UNDECLARED_WALL_COLOR,
        Color::BLACK
    )),
    m_undeclaredNoWallColor(getColor(
        KEY_UNDECLARED_NO_WALL_COLOR,
        Color::BLACK
    )),
    m_incorrectlyDeclaredWallColor(getColor(
        KEY_INCORRECTLY_DECLARED_WALL_COLOR,
        Color::RED
    )),
    m_incorrectlyDeclaredNoWallColor(getColor(
        KEY_INCORRECTLY_DECLARED_NO_WALL_COLOR,
        Color::BLACK
    )),
    m_mouseBodyColor(getColor(
        KEY_MOUSE_BODY_COLOR,
        Color::BLUE
    )),
    m_mouseWheelColor(getColor(
        KEY_MOUSE_WHEEL_COLOR,
        Color::GREEN
    )) {
}

Color ColorManager::getColor(const QString& key, Color defaultColor) {
    QString value = getValue(key);
    if (!STRING_TO_COLOR().contains(value)) {
        value = COLOR_TO_STRING().value(defaultColor);
        setValue(key, value);
    }
    return STRING_TO_COLOR().value(value);
}

QString ColorManager::getValue(const QString& key) {
    return Settings::get()->value(GROUP, key);
}

void ColorManager::setValue(const QString& key, const QString& value) {
    Settings::get()->update(GROUP, key, value);
}

} // namespace mms

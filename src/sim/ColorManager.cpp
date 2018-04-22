#include "ColorManager.h"

#include "Assert.h"
#include "Logging.h"
#include "Settings.h"
#include "SimUtilities.h"

namespace mms {

const QString ColorManager::GROUP = "colors";
const QString ColorManager::KEY_TILE_BASE_COLOR = "tile-base-color";
const QString ColorManager::KEY_TILE_WALL_COLOR = "tile-wall-color";
const QString ColorManager::KEY_TILE_CORNER_COLOR = "tile-corner-color";
const QString ColorManager::KEY_TILE_FOG_COLOR = "tile-fog-color";
const QString ColorManager::KEY_TILE_FOG_ALPHA = "tile-fog-alpha";
const QString ColorManager::KEY_UNDECLARED_WALL_COLOR = 
    "undeclared-wall-color";
const QString ColorManager::KEY_UNDECLARED_NO_WALL_COLOR =
    "undeclared-no-wall-color";
const QString ColorManager::KEY_INCORRECTLY_DECLARED_WALL_COLOR =
    "incorrectly-declared-wall-color";
const QString ColorManager::KEY_INCORRECTLY_DECLARED_NO_WALL_COLOR =
    "incorrectly-declared-no-wall-color";
const QString ColorManager::KEY_DISTANCE_CORRECT_TILE_BASE_COLOR =
    "distance-correct-tile-base-color";
const QString ColorManager::KEY_MOUSE_BODY_COLOR = "mouse-body-color";
const QString ColorManager::KEY_MOUSE_CENTER_OF_MASS_COLOR =
    "mouse-center-of-mass-color";
const QString ColorManager::KEY_MOUSE_WHEEL_COLOR = "mouse-wheel-color";
const QString ColorManager::KEY_MOUSE_SENSOR_COLOR = "mouse-sensor-color";
const QString ColorManager::KEY_MOUSE_VISION_COLOR = "mouse-vision-color";

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

Color ColorManager::getTileFogColor() {
    return m_tileFogColor;
}

double ColorManager::getTileFogAlpha() {
    return m_tileFogAlpha;
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

Color ColorManager::getDistanceCorrectTileBaseColor() {
    return m_distanceCorrectTileBaseColor;
}

Color ColorManager::getMouseBodyColor() {
    return m_mouseBodyColor;
}

Color ColorManager::getMouseCenterOfMassColor() {
    return m_mouseCenterOfMassColor;
}

Color ColorManager::getMouseWheelColor() {
    return m_mouseWheelColor;
}

Color ColorManager::getMouseSensorColor() {
    return m_mouseSensorColor;
}

Color ColorManager::getMouseVisionColor() {
    return m_mouseVisionColor;
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
    m_tileFogColor(getColor(
        KEY_TILE_FOG_COLOR,
        Color::GRAY
    )),
    m_undeclaredWallColor(getColor(
        KEY_UNDECLARED_WALL_COLOR,
        Color::DARK_RED
    )),
    m_undeclaredNoWallColor(getColor(
        KEY_UNDECLARED_NO_WALL_COLOR,
        Color::DARK_GRAY
    )),
    m_incorrectlyDeclaredWallColor(getColor(
        KEY_INCORRECTLY_DECLARED_WALL_COLOR,
        Color::ORANGE
    )),
    m_incorrectlyDeclaredNoWallColor(getColor(
        KEY_INCORRECTLY_DECLARED_NO_WALL_COLOR,
        Color::DARK_CYAN
    )),
    m_distanceCorrectTileBaseColor(getColor(
        KEY_DISTANCE_CORRECT_TILE_BASE_COLOR,
        Color::DARK_YELLOW
    )),
    m_mouseBodyColor(getColor(
        KEY_MOUSE_BODY_COLOR,
        Color::BLUE
    )),
    m_mouseCenterOfMassColor(getColor(
        KEY_MOUSE_CENTER_OF_MASS_COLOR,
        Color::ORANGE
    )),
    m_mouseWheelColor(getColor(
        KEY_MOUSE_WHEEL_COLOR,
        Color::GREEN
    )),
    m_mouseSensorColor(getColor(
        KEY_MOUSE_SENSOR_COLOR,
        Color::GREEN
    )),
    m_mouseVisionColor(getColor(
        KEY_MOUSE_VISION_COLOR,
        Color::WHITE
    )) {

    QString defaultValue = "0.15";
    QString value = getValue(KEY_TILE_FOG_ALPHA);
    if (!SimUtilities::isDouble(value)) {
        value = defaultValue;
        setValue(KEY_TILE_FOG_ALPHA, value);
    }
    double number = SimUtilities::strToDouble(value);
    if (number < 0.0 || 1.0 < number) {
        number = SimUtilities::strToDouble(defaultValue);
        setValue(KEY_TILE_FOG_ALPHA, defaultValue);
    }
    m_tileFogAlpha = number;
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

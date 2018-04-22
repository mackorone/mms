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

void ColorManager::setTileBaseColor(Color color) {
    setColor(KEY_TILE_BASE_COLOR, color);
    m_tileBaseColor = color;
}

Color ColorManager::getTileWallColor() {
    return m_tileWallColor;
}

void ColorManager::setTileWallColor(Color color) {
    setColor(KEY_TILE_WALL_COLOR, color);
    m_tileWallColor = color;
}

Color ColorManager::getTileCornerColor() {
    return m_tileCornerColor;
}

void ColorManager::setTileCornerColor(Color color) {
    setColor(KEY_TILE_CORNER_COLOR, color);
    m_tileCornerColor = color;
}

Color ColorManager::getTileFogColor() {
    return m_tileFogColor;
}

void ColorManager::setTileFogColor(Color color) {
    setColor(KEY_TILE_FOG_COLOR, color);
    m_tileFogColor = color;
}

double ColorManager::getTileFogAlpha() {
    return m_tileFogAlpha;
}

void ColorManager::setTileFogAlpha(double alpha) {
    setValue(KEY_TILE_FOG_ALPHA, QString::number(alpha));
}

Color ColorManager::getUndeclaredWallColor() {
    return m_undeclaredWallColor;
}

void ColorManager::setUndeclaredWallColor(Color color) {
    setColor(KEY_UNDECLARED_WALL_COLOR, color);
    m_undeclaredWallColor = color;
}

Color ColorManager::getUndeclaredNoWallColor() {
    return m_undeclaredNoWallColor;
}

void ColorManager::setUndeclaredNoWallColor(Color color) {
    setColor(KEY_UNDECLARED_NO_WALL_COLOR, color);
    m_undeclaredNoWallColor = color;
}

Color ColorManager::getIncorrectlyDeclaredWallColor() {
    return m_incorrectlyDeclaredWallColor;
}

void ColorManager::setIncorrectlyDeclaredWallColor(Color color) {
    setColor(KEY_INCORRECTLY_DECLARED_WALL_COLOR, color);
    m_incorrectlyDeclaredWallColor = color;
}

Color ColorManager::getIncorrectlyDeclaredNoWallColor() {
    return m_incorrectlyDeclaredNoWallColor;
}

void ColorManager::setIncorrectlyDeclaredNoWallColor(Color color) {
    setColor(KEY_INCORRECTLY_DECLARED_NO_WALL_COLOR, color);
    m_incorrectlyDeclaredNoWallColor = color;
}

Color ColorManager::getDistanceCorrectTileBaseColor() {
    return m_distanceCorrectTileBaseColor;
}

void ColorManager::setDistanceCorrectTileBaseColor(Color color) {
    setColor(KEY_DISTANCE_CORRECT_TILE_BASE_COLOR, color);
    m_distanceCorrectTileBaseColor = color;
}

Color ColorManager::getMouseBodyColor() {
    return m_mouseBodyColor;
}

void ColorManager::setMouseBodyColor(Color color) {
    setColor(KEY_MOUSE_BODY_COLOR, color);
    m_mouseBodyColor = color;
}

Color ColorManager::getMouseCenterOfMassColor() {
    return m_mouseCenterOfMassColor;
}

void ColorManager::setMouseCenterOfMassColor(Color color) {
    setColor(KEY_MOUSE_CENTER_OF_MASS_COLOR, color);
    m_mouseCenterOfMassColor = color;
}

Color ColorManager::getMouseWheelColor() {
    return m_mouseWheelColor;
}

void ColorManager::setMouseWheelColor(Color color) {
    setColor(KEY_MOUSE_WHEEL_COLOR, color);
    m_mouseWheelColor = color;
}

Color ColorManager::getMouseSensorColor() {
    return m_mouseSensorColor;
}

void ColorManager::setMouseSensorColor(Color color) {
    setColor(KEY_MOUSE_SENSOR_COLOR, color);
    m_mouseSensorColor = color;
}

Color ColorManager::getMouseVisionColor() {
    return m_mouseVisionColor;
}

void ColorManager::setMouseVisionColor(Color color) {
    setColor(KEY_MOUSE_VISION_COLOR, color);
    m_mouseVisionColor = color;
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
        setColor(key, defaultColor);
        value = COLOR_TO_STRING().value(defaultColor);
    }
    return STRING_TO_COLOR().value(value);
}

void ColorManager::setColor(const QString& key, Color color) {
    setValue(key, COLOR_TO_STRING().value(color));
}

QString ColorManager::getValue(const QString& key) {
    return Settings::get()->value(GROUP, key);
}

void ColorManager::setValue(const QString& key, const QString& value) {
    Settings::get()->update(GROUP, key, value);
}

} // namespace mms

#include "SettingsRecent.h"

#include "Settings.h"
#include "SimUtilities.h"

namespace mms {

const QString SettingsRecent::GROUP = "recent";
const QString SettingsRecent::KEY_MAZE_ALGO = "maze-algo";
const QString SettingsRecent::KEY_MOUSE_ALGO = "mouse-algo";
const QString SettingsRecent::KEY_WINDOW_WIDTH = "window-width";
const QString SettingsRecent::KEY_WINDOW_HEIGHT = "window-height";

QString SettingsRecent::getRecentMazeAlgo() {
    return getValue(KEY_MAZE_ALGO);
}

void SettingsRecent::setRecentMazeAlgo(const QString& name) {
    setValue(KEY_MAZE_ALGO, name);
}

QString SettingsRecent::getRecentMouseAlgo() {
    return getValue(KEY_MOUSE_ALGO);
}

void SettingsRecent::setRecentMouseAlgo(const QString& name) {
    setValue(KEY_MOUSE_ALGO, name);
}

int SettingsRecent::getRecentWindowWidth() {
    return getNumber(KEY_WINDOW_WIDTH, 1200);
}

void SettingsRecent::setRecentWindowWidth(int width) {
    setValue(KEY_WINDOW_WIDTH, QString::number(width));
}

int SettingsRecent::getRecentWindowHeight() {
    return getNumber(KEY_WINDOW_HEIGHT, 600);
}

void SettingsRecent::setRecentWindowHeight(int height) {
    setValue(KEY_WINDOW_HEIGHT, QString::number(height));
}

int SettingsRecent::getNumber(QString key, int defaultValue) {
    int number = -1;
    QString value = getValue(key);
    if (SimUtilities::isInt(value)) {
        number = SimUtilities::strToInt(value);
    }
    if (number < 0) {
        number = defaultValue;
        setValue(key, QString::number(number));
    }
    return number;
}

QString SettingsRecent::getValue(const QString& key) {
    return Settings::get()->value(GROUP, key);
}

void SettingsRecent::setValue(const QString& key, const QString& value) {
    Settings::get()->update(GROUP, key, value);
}

} // namespace mms

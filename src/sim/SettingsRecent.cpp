#include "SettingsRecent.h"

#include "Settings.h"

namespace mms {

const QString SettingsRecent::GROUP = "recent";
const QString SettingsRecent::KEY_MAZE_ALGO = "maze-algo";
const QString SettingsRecent::KEY_MOUSE_ALGO = "mouse-algo";

QString SettingsRecent::getRecentMazeAlgo() {
    return getValue(KEY_MAZE_ALGO);
}

QString SettingsRecent::getRecentMouseAlgo() {
    return getValue(KEY_MOUSE_ALGO);
}

void SettingsRecent::setRecentMazeAlgo(const QString& name) {
    setValue(KEY_MAZE_ALGO, name);
}

void SettingsRecent::setRecentMouseAlgo(const QString& name) {
    setValue(KEY_MOUSE_ALGO, name);
}

QString SettingsRecent::getValue(const QString& key) {
    return Settings::get()->value(GROUP, key);
}

void SettingsRecent::setValue(const QString& key, const QString& value) {
    Settings::get()->update(GROUP, key, value);
}

} // namespace mms

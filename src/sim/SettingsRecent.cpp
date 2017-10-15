#include "SettingsRecent.h"

#include "Settings.h"

namespace mms {

const QString SettingsRecent::GROUP_PREFIX = "recent";
const QString SettingsRecent::MAZE_ALGO_KEY = "maze-algo";
const QString SettingsRecent::MOUSE_ALGO_KEY = "mouse-algo";

QString SettingsRecent::getRecentMazeAlgo() {
    return getValue(MAZE_ALGO_KEY);
}

QString SettingsRecent::getRecentMouseAlgo() {
    return getValue(MOUSE_ALGO_KEY);
}

void SettingsRecent::setRecentMazeAlgo(const QString& name) {
    setValue(MAZE_ALGO_KEY, name);
}

void SettingsRecent::setRecentMouseAlgo(const QString& name) {
    setValue(MOUSE_ALGO_KEY, name);
}

QString SettingsRecent::getValue(const QString& key) {
    return Settings::get()->value(GROUP_PREFIX, key);
}

void SettingsRecent::setValue(const QString& key, const QString& value) {
    Settings::get()->update(GROUP_PREFIX, key, value);
}

} // namespace mms

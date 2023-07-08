#include "SettingsMisc.h"

#include "Settings.h"

namespace mms {

const QString SettingsMisc::GROUP = "misc";
const QString SettingsMisc::KEY_RECENT_MAZE_ALGO = "recent-maze-file";
const QString SettingsMisc::KEY_RECENT_MOUSE_ALGO = "recent-mouse-algo";
const QString SettingsMisc::KEY_RECENT_WINDOW_WIDTH = "recent-window-width";
const QString SettingsMisc::KEY_RECENT_WINDOW_HEIGHT = "recent-window-height";

QString SettingsMisc::getRecentMazeFile() {
  return getValue(KEY_RECENT_MAZE_ALGO);
}

void SettingsMisc::setRecentMazeFile(const QString &path) {
  setValue(KEY_RECENT_MAZE_ALGO, path);
}

QString SettingsMisc::getRecentMouseAlgo() {
  return getValue(KEY_RECENT_MOUSE_ALGO);
}

void SettingsMisc::setRecentMouseAlgo(const QString &name) {
  setValue(KEY_RECENT_MOUSE_ALGO, name);
}

int SettingsMisc::getRecentWindowWidth() {
  return getNumber(KEY_RECENT_WINDOW_WIDTH, 1200);
}

void SettingsMisc::setRecentWindowWidth(int width) {
  setValue(KEY_RECENT_WINDOW_WIDTH, QString::number(width));
}

int SettingsMisc::getRecentWindowHeight() {
  return getNumber(KEY_RECENT_WINDOW_HEIGHT, 600);
}

void SettingsMisc::setRecentWindowHeight(int height) {
  setValue(KEY_RECENT_WINDOW_HEIGHT, QString::number(height));
}

int SettingsMisc::getNumber(QString key, int defaultValue) {
  bool ok = true;
  int number = getValue(key).toInt(&ok);
  if (!ok) {
    number = defaultValue;
    setValue(key, QString::number(number));
  }
  return number;
}

QString SettingsMisc::getValue(const QString &key) {
  return Settings::get()->value(GROUP, key);
}

void SettingsMisc::setValue(const QString &key, const QString &value) {
  Settings::get()->update(GROUP, key, value);
}

}  // namespace mms

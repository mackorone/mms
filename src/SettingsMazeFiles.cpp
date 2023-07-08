#include "SettingsMazeFiles.h"

#include "Settings.h"

namespace mms {

const QString SettingsMazeFiles::GROUP = "maze-files";
const QString SettingsMazeFiles::KEY_PATH = "path";

QStringList SettingsMazeFiles::getAllPaths() {
  return Settings::get()->values(GROUP, KEY_PATH);
}

void SettingsMazeFiles::addPath(QString path) {
  if (Settings::get()->find(GROUP, KEY_PATH, path).isEmpty()) {
    Settings::get()->add(GROUP, {{KEY_PATH, path}});
  }
}

void SettingsMazeFiles::removePath(QString path) {
  Settings::get()->remove(GROUP, KEY_PATH, path);
}

}  // namespace mms

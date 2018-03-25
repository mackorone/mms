#include "SettingsMazeFiles.h"

#include "Assert.h"
#include "Settings.h"

namespace mms {

const QString SettingsMazeFiles::GROUP = "mazeFiles";
const QString SettingsMazeFiles::KEY_PATH = "path";

QStringList SettingsMazeFiles::getSettingsMazeFiles() {
    return Settings::get()->values(GROUP, KEY_PATH);
}

void SettingsMazeFiles::addMazeFile(const QString& path) {
    Settings::get()->add(GROUP, {
        {KEY_PATH, path},
    });
}

void SettingsMazeFiles::removeMazeFile(const QString& path) {
    Settings::get()->remove(GROUP, KEY_PATH, path);
}

} //namespace mms

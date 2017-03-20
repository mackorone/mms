#include "SettingsMazeFiles.h"

#include "Assert.h"
#include "Settings.h"

namespace mms {

const QString SettingsMazeFiles::GROUP_PREFIX = "mazeFiles";
const QString SettingsMazeFiles::PATH_KEY = "path";

QStringList SettingsMazeFiles::getSettingsMazeFiles() {
    return Settings::get()->values(GROUP_PREFIX, PATH_KEY);
}

void SettingsMazeFiles::addMazeFile(const QString& path) {
    Settings::get()->add(GROUP_PREFIX, {
        {PATH_KEY, path},
    });
}

void SettingsMazeFiles::removeMazeFile(const QString& path) {
    Settings::get()->remove(GROUP_PREFIX, PATH_KEY, path);
}

} //namespace mms

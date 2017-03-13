#include "MazeFiles.h"

#include "Assert.h"
#include "Settings.h"

namespace mms {

const QString MazeFiles::GROUP_PREFIX = "mazeFiles";
const QString MazeFiles::PATH_KEY = "path";

QStringList MazeFiles::getMazeFiles() {
    return Settings::get()->values(GROUP_PREFIX, PATH_KEY);
}

void MazeFiles::addMazeFile(const QString& path) {
    Settings::get()->add(GROUP_PREFIX, {
        {PATH_KEY, path},
    });
}

void MazeFiles::removeMazeFile(const QString& path) {
    Settings::get()->remove(GROUP_PREFIX, PATH_KEY, path);
}

} //namespace mms

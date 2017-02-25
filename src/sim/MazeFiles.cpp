#include "MazeFiles.h"

#include <QSettings>

#include "Assert.h"

namespace mms {

const QString MazeFiles::GROUP_PREFIX = "mazeFiles";
const QString MazeFiles::PATH_KEY = "path";

QStringList MazeFiles::getMazeFiles() {
    QStringList paths;
    QSettings settings;
    int size = settings.beginReadArray(GROUP_PREFIX);
    for (int i = 0; i < size; i += 1) {
        settings.setArrayIndex(i);
        paths << settings.value(PATH_KEY).toString();
    }
    settings.endArray();
    paths.sort(Qt::CaseInsensitive);
    return paths;
}

void MazeFiles::addMazeFile(const QString& path) {

    QSettings settings;

    // Do some sanity checks
    int size = settings.beginReadArray(GROUP_PREFIX);
    for (int i = 0; i < size; i += 1) {
        settings.setArrayIndex(i);
        if (path == settings.value(PATH_KEY).toString()) {
            // TODO: MACK
            return;
        }
    }
    settings.endArray();

    // Write the new algo
    settings.beginWriteArray(GROUP_PREFIX);
    settings.setArrayIndex(size);
    settings.setValue(PATH_KEY, path);
    settings.endArray();
}

} //namespace mms

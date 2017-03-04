#include "MazeAlgos.h"

#include <QSettings>

#include "Assert.h"

namespace mms {

const QString MazeAlgos::GROUP_PREFIX = "mazeAlgos";
const QString MazeAlgos::NAME_KEY = "name";
const QString MazeAlgos::DIR_PATH_KEY = "dirPath";
const QString MazeAlgos::BUILD_COMMAND_KEY = "buildCommand";
const QString MazeAlgos::RUN_COMMAND_KEY = "runCommand";

QStringList MazeAlgos::algoNames() {
    QStringList names;
    QSettings settings;
    int size = settings.beginReadArray(GROUP_PREFIX);
    for (int i = 0; i < size; i += 1) {
        settings.setArrayIndex(i);
        names << settings.value(NAME_KEY).toString();
    }
    settings.endArray();
    names.sort(Qt::CaseInsensitive);
    return names;
}

QString MazeAlgos::getDirPath(const QString& name) {
    return get(name, DIR_PATH_KEY);
}

QString MazeAlgos::getBuildCommand(const QString& name) {
    return get(name, BUILD_COMMAND_KEY);
}

QString MazeAlgos::getRunCommand(const QString& name) {
    return get(name, RUN_COMMAND_KEY);
}

void MazeAlgos::addAlgo(
        const QString& name,
        const QString& dirPath,
        const QString& buildCommand,
        const QString& runCommand) {

    QSettings settings;

    // Do some sanity checks
    int size = settings.beginReadArray(GROUP_PREFIX);
    for (int i = 0; i < size; i += 1) {
        settings.setArrayIndex(i);
        if (name == settings.value(NAME_KEY).toString()) {
            // TODO: MACK
            return;
        }
        if (dirPath == settings.value(DIR_PATH_KEY).toString()) {
            // TODO: MACK
            return;
        }
    }
    settings.endArray();

    // Write the new algo
    settings.beginWriteArray(GROUP_PREFIX);
    settings.setArrayIndex(size);
    settings.setValue(NAME_KEY, name);
    settings.setValue(DIR_PATH_KEY, dirPath);
    settings.setValue(BUILD_COMMAND_KEY, buildCommand);
    settings.setValue(RUN_COMMAND_KEY, runCommand);
    settings.endArray();
}

QString MazeAlgos::get(const QString& name, const QString& key) {
    QSettings settings;
    int size = settings.beginReadArray(GROUP_PREFIX);
    for (int i = 0; i < size; i += 1) {
        settings.setArrayIndex(i);
        if (name == settings.value(NAME_KEY)) {
            // TODO: MACK - assert it has this
            return settings.value(key).toString();
        }
    }
    ASSERT_NEVER_RUNS();    
}

} //namespace mms

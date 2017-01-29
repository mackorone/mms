#include "MouseAlgos.h"

#include <QSettings>

#include "Assert.h"

namespace mms {

const QString MouseAlgos::GROUP_PREFIX = "mouseAlgos";
const QString MouseAlgos::NAME_KEY = "name";
const QString MouseAlgos::DIR_PATH_KEY = "dirPath";
const QString MouseAlgos::BUILD_COMMAND_KEY = "buildCommand";
const QString MouseAlgos::RUN_COMMAND_KEY = "runCommand";

QStringList MouseAlgos::algoNames() {
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

QString MouseAlgos::getDirPath(const QString& name) {
    return get(name, DIR_PATH_KEY);
}

QString MouseAlgos::getBuildCommand(const QString& name) {
    return get(name, BUILD_COMMAND_KEY);
}

QString MouseAlgos::getRunCommand(const QString& name) {
    return get(name, RUN_COMMAND_KEY);
}

void MouseAlgos::addAlgo(
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

QString MouseAlgos::get(const QString& name, const QString& key) {
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

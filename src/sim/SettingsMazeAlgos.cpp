#include "SettingsMazeAlgos.h"

#include "Assert.h"
#include "Settings.h"

namespace mms {

const QString SettingsMazeAlgos::GROUP_PREFIX = "mazeAlgos";
const QString SettingsMazeAlgos::NAME_KEY = "name";
const QString SettingsMazeAlgos::DIR_PATH_KEY = "dirPath";
const QString SettingsMazeAlgos::BUILD_COMMAND_KEY = "buildCommand";
const QString SettingsMazeAlgos::RUN_COMMAND_KEY = "runCommand";

QStringList SettingsMazeAlgos::names() {
    return Settings::get()->values(GROUP_PREFIX, NAME_KEY);
}

QString SettingsMazeAlgos::getDirPath(const QString& name) {
    return getValue(name, DIR_PATH_KEY);
}

QString SettingsMazeAlgos::getBuildCommand(const QString& name) {
    return getValue(name, BUILD_COMMAND_KEY);
}

QString SettingsMazeAlgos::getRunCommand(const QString& name) {
    return getValue(name, RUN_COMMAND_KEY);
}

void SettingsMazeAlgos::add(
    const QString& name,
    const QString& dirPath,
    const QString& buildCommand,
    const QString& runCommand
) {
    Settings::get()->add(GROUP_PREFIX, {
        {NAME_KEY, name},
        {DIR_PATH_KEY, dirPath},
        {BUILD_COMMAND_KEY, buildCommand},
        {RUN_COMMAND_KEY, runCommand},
    });
}

void SettingsMazeAlgos::update(
    const QString& name,
    const QString& newName,
    const QString& newDirPath,
    const QString& newBuildCommand,
    const QString& newRunCommand
) {
    Settings::get()->update(GROUP_PREFIX, NAME_KEY, name, {
        {NAME_KEY, newName},
        {DIR_PATH_KEY, newDirPath},
        {BUILD_COMMAND_KEY, newBuildCommand},
        {RUN_COMMAND_KEY, newRunCommand},
    });
}

void SettingsMazeAlgos::remove(const QString& name) {
    Settings::get()->remove(
        GROUP_PREFIX,
        NAME_KEY,
        name
    );
}

QString SettingsMazeAlgos::getValue(const QString& name, const QString& key) {
    const auto& vector = Settings::get()->find(GROUP_PREFIX, NAME_KEY, name);
    return (vector.size() == 0 ? "" : vector.at(0).value(key));
}

} //namespace mms

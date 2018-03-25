#include "SettingsMazeAlgos.h"

#include "Assert.h"
#include "Settings.h"

namespace mms {

const QString SettingsMazeAlgos::GROUP = "mazeAlgos";
const QString SettingsMazeAlgos::KEY_NAME = "name";
const QString SettingsMazeAlgos::KEY_DIR_PATH = "dirPath";
const QString SettingsMazeAlgos::KEY_BUILD_COMMAND = "buildCommand";
const QString SettingsMazeAlgos::KEY_RUN_COMMAND = "runCommand";

QStringList SettingsMazeAlgos::names() {
    return Settings::get()->values(GROUP, KEY_NAME);
}

QString SettingsMazeAlgos::getDirPath(const QString& name) {
    return getValue(name, KEY_DIR_PATH);
}

QString SettingsMazeAlgos::getBuildCommand(const QString& name) {
    return getValue(name, KEY_BUILD_COMMAND);
}

QString SettingsMazeAlgos::getRunCommand(const QString& name) {
    return getValue(name, KEY_RUN_COMMAND);
}

void SettingsMazeAlgos::add(
    const QString& name,
    const QString& dirPath,
    const QString& buildCommand,
    const QString& runCommand
) {
    Settings::get()->add(GROUP, {
        {KEY_NAME, name},
        {KEY_DIR_PATH, dirPath},
        {KEY_BUILD_COMMAND, buildCommand},
        {KEY_RUN_COMMAND, runCommand},
    });
}

void SettingsMazeAlgos::update(
    const QString& name,
    const QString& newName,
    const QString& newDirPath,
    const QString& newBuildCommand,
    const QString& newRunCommand
) {
    Settings::get()->update(GROUP, KEY_NAME, name, {
        {KEY_NAME, newName},
        {KEY_DIR_PATH, newDirPath},
        {KEY_BUILD_COMMAND, newBuildCommand},
        {KEY_RUN_COMMAND, newRunCommand},
    });
}

void SettingsMazeAlgos::remove(const QString& name) {
    Settings::get()->remove(
        GROUP,
        KEY_NAME,
        name
    );
}

QString SettingsMazeAlgos::getValue(const QString& name, const QString& key) {
    const auto& vector = Settings::get()->find(GROUP, KEY_NAME, name);
    return (vector.size() == 0 ? "" : vector.at(0).value(key));
}

} //namespace mms

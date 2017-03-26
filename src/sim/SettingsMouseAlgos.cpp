#include "SettingsMouseAlgos.h"

#include "Assert.h"
#include "ConfigDialog.h"
#include "ConfigDialogField.h"
#include "Settings.h"

namespace mms {

const QString SettingsMouseAlgos::GROUP_PREFIX = "mouseAlgos";
const QString SettingsMouseAlgos::NAME_KEY = "name";
const QString SettingsMouseAlgos::DIR_PATH_KEY = "dirPath";
const QString SettingsMouseAlgos::BUILD_COMMAND_KEY = "buildCommand";
const QString SettingsMouseAlgos::RUN_COMMAND_KEY = "runCommand";
const QString SettingsMouseAlgos::MOUSE_FILE_PATH_KEY = "mouseFilePath";

QStringList SettingsMouseAlgos::names() {
    return Settings::get()->values(GROUP_PREFIX, NAME_KEY);
}

QString SettingsMouseAlgos::getDirPath(const QString& name) {
    return getValue(name, DIR_PATH_KEY);
}

QString SettingsMouseAlgos::getBuildCommand(const QString& name) {
    return getValue(name, BUILD_COMMAND_KEY);
}

QString SettingsMouseAlgos::getRunCommand(const QString& name) {
    return getValue(name, RUN_COMMAND_KEY);
}

QString SettingsMouseAlgos::getMouseFilePath(const QString& name) {
    return getValue(name, MOUSE_FILE_PATH_KEY);
}

void SettingsMouseAlgos::add(
    const QString& name,
    const QString& dirPath,
    const QString& buildCommand,
    const QString& runCommand,
    const QString& mouseFilePath
) {
    Settings::get()->add(GROUP_PREFIX, {
        {NAME_KEY, name},
        {DIR_PATH_KEY, dirPath},
        {BUILD_COMMAND_KEY, buildCommand},
        {RUN_COMMAND_KEY, runCommand},
        {MOUSE_FILE_PATH_KEY, mouseFilePath},
    });
}

void SettingsMouseAlgos::update(
    const QString& name,
    const QString& newName,
    const QString& newDirPath,
    const QString& newBuildCommand,
    const QString& newRunCommand,
    const QString& newMouseFilePath
) {
    Settings::get()->update(GROUP_PREFIX, NAME_KEY, name, {
        {NAME_KEY, newName},
        {DIR_PATH_KEY, newDirPath},
        {BUILD_COMMAND_KEY, newBuildCommand},
        {RUN_COMMAND_KEY, newRunCommand},
        {MOUSE_FILE_PATH_KEY, newMouseFilePath},
    });
}

void SettingsMouseAlgos::remove(const QString& name) {
    Settings::get()->remove(
        GROUP_PREFIX,
        NAME_KEY,
        name
    );
}

QString SettingsMouseAlgos::getValue(const QString& name, const QString& key) {
    const auto& vector = Settings::get()->find(GROUP_PREFIX, NAME_KEY, name);
    return (vector.size() == 0 ? "" : vector.at(0).value(key));
}

} //namespace mms

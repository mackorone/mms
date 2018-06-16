#include "SettingsMouseAlgos.h"

#include "Assert.h"
#include "ConfigDialog.h"
#include "ConfigDialogField.h"
#include "Settings.h"

namespace mms {

const QString SettingsMouseAlgos::GROUP = "mouseAlgos";
const QString SettingsMouseAlgos::KEY_NAME = "name";
const QString SettingsMouseAlgos::KEY_DIR_PATH = "dirPath";
const QString SettingsMouseAlgos::KEY_BUILD_COMMAND = "buildCommand";
const QString SettingsMouseAlgos::KEY_RUN_COMMAND = "runCommand";

QStringList SettingsMouseAlgos::names() {
    return Settings::get()->values(GROUP, KEY_NAME);
}

QString SettingsMouseAlgos::getDirPath(const QString& name) {
    return getValue(name, KEY_DIR_PATH);
}

QString SettingsMouseAlgos::getBuildCommand(const QString& name) {
    return getValue(name, KEY_BUILD_COMMAND);
}

QString SettingsMouseAlgos::getRunCommand(const QString& name) {
    return getValue(name, KEY_RUN_COMMAND);
}

void SettingsMouseAlgos::add(
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

void SettingsMouseAlgos::update(
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

void SettingsMouseAlgos::remove(const QString& name) {
    Settings::get()->remove(
        GROUP,
        KEY_NAME,
        name
    );
}

QString SettingsMouseAlgos::getValue(const QString& name, const QString& key) {
    const auto& vector = Settings::get()->find(GROUP, KEY_NAME, name);
    return (vector.size() == 0 ? "" : vector.at(0).value(key));
}

} //namespace mms

#include "MouseAlgos.h"

#include "Assert.h"
#include "Settings.h"

namespace mms {

const QString MouseAlgos::GROUP_PREFIX = "mouseAlgos";
const QString MouseAlgos::NAME_KEY = "name";
const QString MouseAlgos::DIR_PATH_KEY = "dirPath";
const QString MouseAlgos::BUILD_COMMAND_KEY = "buildCommand";
const QString MouseAlgos::RUN_COMMAND_KEY = "runCommand";
const QString MouseAlgos::MOUSE_FILE_PATH_KEY = "mouseFilePath";

QStringList MouseAlgos::algoNames() {
    return Settings::get()->values(GROUP_PREFIX, NAME_KEY);
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

QString MouseAlgos::getMouseFilePath(const QString& name) {
    return get(name, MOUSE_FILE_PATH_KEY);
}

void MouseAlgos::addAlgo(
        const QString& name,
        const QString& dirPath,
        const QString& buildCommand,
        const QString& runCommand,
        const QString& mouseFilePath) {
    Settings::get()->add(GROUP_PREFIX, {
        {NAME_KEY, name},
        {DIR_PATH_KEY, dirPath},
        {BUILD_COMMAND_KEY, buildCommand},
        {RUN_COMMAND_KEY, runCommand},
        {MOUSE_FILE_PATH_KEY, mouseFilePath},
    });
}

void MouseAlgos::updateAlgo(
        const QString& name,
        const QString& newName,
        const QString& newDirPath,
        const QString& newBuildCommand,
        const QString& newRunCommand,
        const QString& newMouseFilePath) {
    Settings::get()->update(GROUP_PREFIX, NAME_KEY, name, {
        {NAME_KEY, newName},
        {DIR_PATH_KEY, newDirPath},
        {BUILD_COMMAND_KEY, newBuildCommand},
        {RUN_COMMAND_KEY, newRunCommand},
        {MOUSE_FILE_PATH_KEY, newMouseFilePath},
    });
}

void MouseAlgos::removeAlgo(const QString& name) {
    Settings::get()->remove(
        GROUP_PREFIX,
        NAME_KEY,
        name
    );
}

QString MouseAlgos::get(const QString& name, const QString& key) {
    const auto& vector = Settings::get()->find(GROUP_PREFIX, NAME_KEY, name);
    ASSERT_EQ(vector.size(), 1); 
    return vector.isEmpty() ? "" : vector.at(0).value(key);
}

} //namespace mms

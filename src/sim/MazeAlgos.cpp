#include "MazeAlgos.h"

#include "Assert.h"
#include "Settings.h"

#include "Logging.h" // TODO: MACK

namespace mms {

const QString MazeAlgos::GROUP_PREFIX = "mazeAlgos";
const QString MazeAlgos::NAME_KEY = "name";
const QString MazeAlgos::DIR_PATH_KEY = "dirPath";
const QString MazeAlgos::BUILD_COMMAND_KEY = "buildCommand";
const QString MazeAlgos::RUN_COMMAND_KEY = "runCommand";

QStringList MazeAlgos::algoNames() {
    return Settings::get()->values(GROUP_PREFIX, NAME_KEY);
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
    Settings::get()->add(GROUP_PREFIX, {
        {NAME_KEY, name},
        {DIR_PATH_KEY, dirPath},
        {BUILD_COMMAND_KEY, buildCommand},
        {RUN_COMMAND_KEY, runCommand},
    });
}

void MazeAlgos::updateAlgo(
        const QString& name,
        const QString& newName,
        const QString& newDirPath,
        const QString& newBuildCommand,
        const QString& newRunCommand) {
    Settings::get()->update(GROUP_PREFIX, NAME_KEY, name, {
        {NAME_KEY, newName},
        {DIR_PATH_KEY, newDirPath},
        {BUILD_COMMAND_KEY, newBuildCommand},
        {RUN_COMMAND_KEY, newRunCommand},
    });
}

void MazeAlgos::removeAlgo(const QString& name) {
    Settings::get()->remove(
        GROUP_PREFIX,
        NAME_KEY,
        name
    );
}

QString MazeAlgos::get(const QString& name, const QString& key) {
    const auto& vector = Settings::get()->find(GROUP_PREFIX, NAME_KEY, name);
    ASSERT_EQ(vector.size(), 1);
    return vector.isEmpty() ? "" : vector.at(0).value(key);
}

} //namespace mms

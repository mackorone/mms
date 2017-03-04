#pragma once

#include <QString>
#include <QStringList>

namespace mms {

class MazeAlgos {

public:

    // TODO: MACK - renaming algos? Changing paths? etc.
    MazeAlgos() = delete;

    static QStringList algoNames();
    static QString getDirPath(const QString& name);
    static QString getBuildCommand(const QString& name);
    static QString getRunCommand(const QString& name);

    static void addAlgo(
        const QString& name,
        const QString& dirPath,
        const QString& buildCommand,
        const QString& runCommand);

private:

    static const QString GROUP_PREFIX;
    static const QString NAME_KEY;
    static const QString DIR_PATH_KEY;
    static const QString BUILD_COMMAND_KEY;
    static const QString RUN_COMMAND_KEY;

    static QString get(const QString& name, const QString& key);

};

} // namespace mms

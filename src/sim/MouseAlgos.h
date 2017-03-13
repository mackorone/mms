#pragma once

#include <QString>
#include <QStringList>

namespace mms {

class MouseAlgos {

public:

    MouseAlgos() = delete;

    static QStringList algoNames();
    static QString getDirPath(const QString& name);
    static QString getBuildCommand(const QString& name);
    static QString getRunCommand(const QString& name);
    static QString getMouseFilePath(const QString& name);

    static void addAlgo(
        const QString& name,
        const QString& dirPath,
        const QString& buildCommand,
        const QString& runCommand,
        const QString& mouseFilePath);

    static void updateAlgo(
        const QString& name,
        const QString& newName,
        const QString& newDirPath,
        const QString& newBuildCommand,
        const QString& newRunCommand,
        const QString& newMouseFilePath);

    static void removeAlgo(const QString& name);


private:

    static const QString GROUP_PREFIX;
    static const QString NAME_KEY;
    static const QString DIR_PATH_KEY;
    static const QString BUILD_COMMAND_KEY;
    static const QString RUN_COMMAND_KEY;
    static const QString MOUSE_FILE_PATH_KEY;

    static QString get(const QString& name, const QString& key);

};

} // namespace mms

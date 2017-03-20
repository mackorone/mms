#pragma once

#include <QString>
#include <QStringList>

namespace mms {

class SettingsMazeFiles {

public:

    SettingsMazeFiles() = delete;
    static QStringList getSettingsMazeFiles();
    static void addMazeFile(const QString& path);
    static void removeMazeFile(const QString& path);

private:

    static const QString GROUP_PREFIX;
    static const QString PATH_KEY;

};

} // namespace mms

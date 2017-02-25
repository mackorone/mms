#pragma once

#include <QString>
#include <QStringList>

namespace mms {

class MazeFiles {

public:

    // TODO: MACK - renaming? Changing paths? Deleting? etc.
    MazeFiles() = delete;

    static QStringList getMazeFiles();
    static void addMazeFile(const QString& path);

private:

    static const QString GROUP_PREFIX;
    static const QString PATH_KEY;

};

} // namespace mms

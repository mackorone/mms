#pragma once

#include <QString>
#include <QStringList>

#include "BasicMaze.h"

namespace mms {

class MazeAlgoUtilities {

public:
    MazeAlgoUtilities() = delete;
    static BasicMaze generate(
        const QString& mazeAlgorithm,
        int width,
        int height);

private:
    static QStringList getMazeAlgos();
    static QPair<QStringList, QStringList> getFiles(const QString& mazeAlgorithm);

};

} // namespace mms

#pragma once

#include <QPair>
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

};

} // namespace mms

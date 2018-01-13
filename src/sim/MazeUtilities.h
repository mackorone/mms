#pragma once

#include <QPair>
#include <QVector>

#include "Direction.h"

namespace mms {

class MazeUtilities {

public:

    MazeUtilities() = delete;

    static QVector<QPair<int, int>> getCenterPositions(int width, int height);
    static QPair<int, int> getMinPosition(QVector<QPair<int, int>> positions);
    static QPair<int, int> positionAfterMovingForward(
        QPair<int, int> position,
        Direction direction);

};

} // namespace mms

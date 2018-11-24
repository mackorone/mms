#include "MazeUtilities.h"

#include "Assert.h"

namespace mms {

QVector<QPair<int, int>> MazeUtilities::getCenterPositions(
    int width,
    int height) {
    QVector<QPair<int, int>> centerPositions;
            centerPositions.append({(width - 1) / 2, (height - 1) / 2});
    if (height % 2 == 0) {
            centerPositions.append({(width - 1) / 2,  height      / 2});
        if (width % 2 == 0) {
            centerPositions.append({ width      / 2, (height - 1) / 2});
            centerPositions.append({ width      / 2,  height      / 2});
        }
    }
    else if (width % 2 == 0) {
            centerPositions.append({ width      / 2, (height - 1) / 2});
    }
    return centerPositions; 
}

QPair<int, int> MazeUtilities::getMinPosition(
    QVector<QPair<int, int>> positions) {
    ASSERT_LT(0, positions.size());
    QPair<int, int> minPosition = positions.at(0);
    for (const auto& pair : positions) {
        if (pair.first < minPosition.first) {
            minPosition = pair;
        }
        if (
            pair.first == minPosition.first &&
            pair.second < minPosition.second
        ) {
            minPosition = pair;
        }
    }
    return minPosition;
}

QPair<int, int> MazeUtilities::positionAfterMovingForward(
    QPair<int, int> position,
    Direction direction) {
    return {
        // X coordinate
        (direction == Direction::EAST ?  position.first + 1 :
        (direction == Direction::WEST ?  position.first - 1 :
        position.first)),
        // Y coordinate
        (direction == Direction::NORTH ?  position.second + 1 :
        (direction == Direction::SOUTH ?  position.second - 1 :
        position.second))
    };
}

} 

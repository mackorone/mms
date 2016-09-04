#pragma once

#include "../IMouseAlgorithm.h"

namespace rightWallFollow {

class RightWallFollow : public IMouseAlgorithm {

public:
    std::string mouseFile() const;
    bool declareWallOnRead() const;
    bool declareBothWallHalves() const;
    bool useTileEdgeMovements() const;
    double wheelSpeedFraction() const;
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, mms::MouseInterface* mouse);

private:
    void rightWallFollowStep(mms::MouseInterface* mouse);
    void justMoveForward(mms::MouseInterface* mouse);
    void turnLeftAndMoveForward(mms::MouseInterface* mouse);
    void turnRightAndMoveForward(mms::MouseInterface* mouse);
    void turnAroundAndMoveForward(mms::MouseInterface* mouse);
};

} // namespace rightWallFollow

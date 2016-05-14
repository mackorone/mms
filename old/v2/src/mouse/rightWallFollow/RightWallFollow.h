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
        char initialDirection, sim::MouseInterface* mouse);

private:
    void rightWallFollowStep(sim::MouseInterface* mouse);
    void justMoveForward(sim::MouseInterface* mouse);
    void turnLeftAndMoveForward(sim::MouseInterface* mouse);
    void turnRightAndMoveForward(sim::MouseInterface* mouse);
    void turnAroundAndMoveForward(sim::MouseInterface* mouse);
};

} // namespace rightWallFollow

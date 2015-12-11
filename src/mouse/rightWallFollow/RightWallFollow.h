#pragma once

#include "../IMouseAlgorithm.h"

namespace rightWallFollow {

class RightWallFollow : public IMouseAlgorithm {

public:
    void solve(
        int mazeWidth, int mazeHeight, bool officialMaze,
        char initialDirection, sim::MouseInterface* mouse);

private:
    void rightWallFollowStep(sim::MouseInterface* mouse);

};

} // namespace rightWallFollow

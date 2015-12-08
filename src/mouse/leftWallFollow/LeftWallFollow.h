#pragma once

#include "../IMouseAlgorithm.h"

namespace leftWallFollow {

class LeftWallFollow : public IMouseAlgorithm {

public:
    void solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse);

private:
    void leftWallFollowStep(sim::MouseInterface* mouse);
};

} // namespace leftWallFollow

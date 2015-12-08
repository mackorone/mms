#pragma once

#include "../IMouseAlgorithm.h"

namespace randomizedWallFollow {

class RandomizedWallFollow : public IMouseAlgorithm{

public:
    void solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse);

private:
    void leftWallFollowStep(sim::MouseInterface* mouse);
    void rightWallFollowStep(sim::MouseInterface* mouse);

};

} // namespace randomizedWallFollow

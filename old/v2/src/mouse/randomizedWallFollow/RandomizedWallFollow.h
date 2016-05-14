#pragma once

#include "../IMouseAlgorithm.h"

namespace randomizedWallFollow {

class RandomizedWallFollow : public IMouseAlgorithm{

public:
    bool declareWallOnRead() const;
    bool declareBothWallHalves() const;
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse);

private:
    void leftWallFollowStep(sim::MouseInterface* mouse);
    void rightWallFollowStep(sim::MouseInterface* mouse);

};

} // namespace randomizedWallFollow

#pragma once

#include "../../IAlgorithm.h"

namespace wallFollow {

class LeftWallFollow : public IAlgorithm {

public:
    void solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse);

private:
    void leftWallFollowStep(sim::MouseInterface* mouse);
};

} // namespace wallFollow

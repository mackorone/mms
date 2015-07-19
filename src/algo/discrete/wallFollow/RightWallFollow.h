#pragma once

#include "../../IAlgorithm.h"

namespace wallFollow {

class RightWallFollow : public IAlgorithm {

public:
    void solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse);

private:
    void rightWallFollowStep(sim::MouseInterface* mouse);

};

} // namespace wallFollow

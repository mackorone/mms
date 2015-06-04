#pragma once

#include "../../IAlgorithm.h"

namespace wallFollow {

class RandomizedWallFollow : public IAlgorithm{

public:
    void solve(sim::MouseInterface* mouse);

private:
    void leftWallFollowStep(sim::MouseInterface* mouse);
    void rightWallFollowStep(sim::MouseInterface* mouse);

};

} // namespace wallFollow

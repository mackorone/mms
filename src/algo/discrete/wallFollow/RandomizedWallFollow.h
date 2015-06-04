#pragma once

#include "../../IAlgorithm.h"

namespace wallFollow {

class RandomizedWallFollow : public IAlgorithm{

public:
    RandomizedWallFollow(sim::MouseInterface** mouse) : IAlgorithm(mouse) {}
    void solve();

private:
    void leftWallFollowStep();
    void rightWallFollowStep();

};

} // namespace wallFollow

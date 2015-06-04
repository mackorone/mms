#pragma once

#include "../../IAlgorithm.h"

namespace wallFollow {

class LeftWallFollow : public IAlgorithm {

public:
    LeftWallFollow(sim::MouseInterface** mouse) : IAlgorithm(mouse) {}
    void solve();

private:
    void leftWallFollowStep();
};

} // namespace wallFollow

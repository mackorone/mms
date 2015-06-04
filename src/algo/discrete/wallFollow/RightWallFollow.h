#pragma once

#include "../../IAlgorithm.h"

namespace wallFollow {

class RightWallFollow : public IAlgorithm{

public:
    RightWallFollow(sim::MouseInterface** mouse) : IAlgorithm(mouse) {}
    void solve();

private:
    void rightWallFollowStep();

};

} // namespace wallFollow

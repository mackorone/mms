#pragma once

#include "../../IAlgorithm.h"

namespace wallFollow {

class RightWallFollow : public IAlgorithm{

public:
    void solve(sim::MouseInterface* mouse);

private:
    void rightWallFollowStep(sim::MouseInterface* mouse);

};

} // namespace wallFollow

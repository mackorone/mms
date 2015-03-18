#pragma once

#include "../IAlgorithm.h"

class RightWallFollow : public IAlgorithm{

public:
    void solve(sim::MouseInterface* mouse);

private:
    void rightWallFollowStep(sim::MouseInterface* mouse);

};

#pragma once

#include "../IAlgorithm.h"

class LeftWallFollow : public IAlgorithm{

public:
    void solve(sim::MouseInterface* mouse);

private:
    void leftWallFollowStep(sim::MouseInterface* mouse);
};

#pragma once

#include "IAlgorithm.h"

class Continue : public IAlgorithm {

public:
    void solve(sim::MouseInterface* mouse);

private:
    bool wallRight(sim::MouseInterface* mouse);
    bool wallFront(sim::MouseInterface* mouse);

    void turnRight(sim::MouseInterface* mouse);
    void turnLeft(sim::MouseInterface* mouse);
    void moveForward(sim::MouseInterface* mouse);

};

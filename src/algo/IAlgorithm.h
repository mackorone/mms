#pragma once

#include "../sim/InterfaceType.h"
#include "../sim/MouseInterface.h"

class IAlgorithm {

public:
    virtual void solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse) = 0;

};

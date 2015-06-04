#pragma once

#include "../sim/InterfaceType.h"
#include "../sim/MouseInterface.h"

class IAlgorithm {

public:
    virtual void solve(sim::MouseInterface* mouse) = 0;

};

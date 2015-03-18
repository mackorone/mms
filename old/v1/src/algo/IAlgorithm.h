#pragma once

#include "../sim/MouseInterface.h"

/*
 * This class is an interface for every algorithm
 */
class IAlgorithm {

public:
    virtual void solve(sim::MouseInterface* mouse) = 0;

};

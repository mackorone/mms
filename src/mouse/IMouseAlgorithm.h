#pragma once

#include "../sim/InterfaceType.h"
#include "../sim/MouseInterface.h"

class IMouseAlgorithm {

public:
    virtual std::string mouseFile() const = 0;
    virtual sim::InterfaceType interfaceType() const = 0;
    virtual void solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse) = 0;
    // TODO: MACK - Add the initial direction that the mouse is facing...

};

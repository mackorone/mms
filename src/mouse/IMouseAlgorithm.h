#pragma once

#include "../sim/MouseInterface.h"

class IMouseAlgorithm {

public:

    // Every algorithm must provide a solve method
    virtual void solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse) = 0;

    // These options are optional, and will take default behavior if not overridden
    virtual std::string mouseFile() const;
    virtual std::string interfaceType() const;
    virtual double wheelSpeedFraction() const;

};

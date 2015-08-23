#pragma once

#include "../sim/MouseInterface.h"

class IMouseAlgorithm {

public:
    virtual ~IMouseAlgorithm();

    virtual std::string mouseFile() const = 0;
    virtual std::string interfaceType() const = 0;
    virtual void solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse) = 0;

};

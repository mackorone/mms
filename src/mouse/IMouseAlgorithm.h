#pragma once

#include "../sim/MouseInterface.h"

class IMouseAlgorithm {

public:

    // Initialization options
    virtual std::string mouseFile() const;
    virtual std::string interfaceType() const;
    virtual std::string initialDirection() const;

    // Runtime options
    virtual bool automaticallyClearFog() const;
    virtual bool declareBothWallHalves() const;
    virtual bool declareWallOnRead() const;
    virtual bool setTileBaseColorWhenDistanceCorrect() const;
    virtual double wheelSpeedFraction() const;

    // Necessary to all algorithms
    virtual void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) = 0;

};

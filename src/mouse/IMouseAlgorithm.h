#pragma once

#include "../sim/MouseInterface.h"

class IMouseAlgorithm {

public:

    // Every algorithm must provide a solve method
    virtual void solve(
        int mazeWidth, int mazeHeight, bool officialMaze,
        char initialDirection, sim::MouseInterface* mouse) = 0;

    // These options are optional, and will take default behavior if not overridden
    // TODO: MACK - document these...
    virtual std::string mouseFile() const;
    virtual std::string interfaceType() const;
    virtual std::string initialDirection() const;

    // TODO: MACK - these are the options
    virtual bool controlTileFog() const;
    virtual bool declareBothWallHalves() const;
    virtual bool declareWallOnRead() const;
    virtual bool setTileBaseColorWhenDistanceCorrect() const;
    virtual double wheelSpeedFraction() const;

};

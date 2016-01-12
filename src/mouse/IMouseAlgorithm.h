#pragma once

#include "../sim/MouseInterface.h"

class IMouseAlgorithm {

public:

    // Initialization options
    virtual std::string mouseFile() const;
    virtual std::string interfaceType() const;
    virtual std::string initialDirection() const;

    // Runtime options
    virtual bool allowOmniscience() const;
    virtual bool automaticallyClearFog() const;
    virtual bool declareWallOnRead() const;
    virtual bool declareBothWallHalves() const;
    virtual bool setTileTextWhenDistanceDeclared() const;
    virtual bool setTileBaseColorWhenDistanceDeclaredCorrectly() const;
    virtual bool stopOnTileEdgesAndAllowSpecialMovements() const;
    virtual double wheelSpeedFraction() const;
    virtual int tileTextNumberOfRows() const;
    virtual int tileTextNumberOfCols() const;

    // Necessary to all algorithms
    virtual void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) = 0;

};

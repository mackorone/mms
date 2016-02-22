#pragma once

#include "../sim/MouseInterface.h"

class IMouseAlgorithm {

public:

    // Initialization options
    virtual std::string mouseFile() const;
    virtual std::string interfaceType() const;
    virtual std::string initialDirection() const;

    // Runtime options for both interface types
    virtual bool allowOmniscience() const;
    virtual bool automaticallyClearFog() const;
    virtual bool declareBothWallHalves() const;
    virtual bool setTileTextWhenDistanceDeclared() const;
    virtual bool setTileBaseColorWhenDistanceDeclaredCorrectly() const;
    virtual int tileTextNumberOfRows() const;
    virtual int tileTextNumberOfCols() const;

    // Runtime options only applicable to the DISCRETE interface
    virtual bool declareWallOnRead() const;
    virtual bool useTileEdgeMovements() const;
    virtual double wheelSpeedFraction() const;

    // Necessary to all algorithms
    virtual void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) = 0;

};

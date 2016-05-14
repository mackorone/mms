#pragma once

#include "../sim/MouseInterface.h"

class IMouseAlgorithm {

public:

    // Static options for both interfaces
    virtual std::string mouseFile() const;
    virtual std::string interfaceType() const;
    virtual std::string initialDirection() const;
    virtual int tileTextNumberOfRows() const;
    virtual int tileTextNumberOfCols() const;

    // Dynamic options for both interface types
    virtual bool allowOmniscience() const;
    virtual bool automaticallyClearFog() const;
    virtual bool declareBothWallHalves() const;
    virtual bool setTileTextWhenDistanceDeclared() const;
    virtual bool setTileBaseColorWhenDistanceDeclaredCorrectly() const;

    // Static options for the DISCRETE interface
    virtual double wheelSpeedFraction() const;

    // Dynamic options for the DISCRETE interface
    virtual bool declareWallOnRead() const;
    virtual bool useTileEdgeMovements() const;

    // Necessary to all algorithms
    virtual void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) = 0;

};

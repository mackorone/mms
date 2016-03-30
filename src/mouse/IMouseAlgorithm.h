#pragma once

#include "../sim/MouseInterface.h"

class IMouseAlgorithm {

public:

    // Initialization options (read once)
    virtual std::string mouseFile() const;
    virtual std::string interfaceType() const;
    virtual std::string initialDirection() const;
    virtual int tileTextNumberOfRows() const;
    virtual int tileTextNumberOfCols() const;

    // Runtime options for both interface types (read many times)
    virtual bool allowOmniscience() const;
    virtual bool automaticallyClearFog() const;
    virtual bool declareBothWallHalves() const;
    virtual bool setTileTextWhenDistanceDeclared() const;
    virtual bool setTileBaseColorWhenDistanceDeclaredCorrectly() const;

    // Runtime options, DISCRETE interface only (read many times)
    virtual bool declareWallOnRead() const;
    virtual bool useTileEdgeMovements() const;
    virtual double wheelSpeedFraction() const;

    // Necessary to all algorithms (called once)
    virtual void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) = 0;

};

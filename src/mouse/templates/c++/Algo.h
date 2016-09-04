#pragma once

#include <string>

#include "Interface.h"

class Algo {

public:

    // Static options for both interfaces
    std::string mouseFile() const;
    std::string interfaceType() const;
    std::string initialDirection() const;
    int tileTextNumberOfRows() const;
    int tileTextNumberOfCols() const;

    // Dynamic options for both interface types
    bool allowOmniscience() const;
    bool automaticallyClearFog() const;
    bool declareBothWallHalves() const;
    bool setTileTextWhenDistanceDeclared() const;
    bool setTileBaseColorWhenDistanceDeclaredCorrectly() const;

    // Static options for the DISCRETE interface
    double wheelSpeedFraction() const;

    // Dynamic options for the DISCRETE interface
    bool declareWallOnRead() const;
    bool useTileEdgeMovements() const;

    // Necessary to all algorithms
    void solve(Interface* interface);

};

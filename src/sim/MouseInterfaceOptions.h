#pragma once

#include "InterfaceType.h"

namespace sim {

struct MouseInterfaceOptions {
    bool allowOmniscience;
    bool setTileTextWhenDistanceDeclared;
    bool setTileBaseColorWhenDistanceDeclaredCorrectly;
    int tileTextNumberOfRows;
    int tileTextNumberOfCols;
    bool declareWallOnRead;
    bool declareBothWallHalves;
    bool useTileEdgeMovements;
    double wheelSpeedFraction;
    InterfaceType interfaceType;
};

} // namespace sim

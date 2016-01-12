#pragma once

#include "InterfaceType.h"

namespace sim {

struct MouseInterfaceOptions {
    bool allowOmniscience;
    bool declareWallOnRead;
    bool declareBothWallHalves;
    bool setTileTextWhenDistanceDeclared;
    bool setTileBaseColorWhenDistanceDeclaredCorrectly;
    bool stopOnTileEdgesAndAllowSpecialMovements;
    double wheelSpeedFraction;
    int tileTextNumberOfRows;
    int tileTextNumberOfCols;
    InterfaceType interfaceType;
};

} // namespace sim

#pragma once

#include "InterfaceType.h"

namespace sim {

struct MouseInterfaceOptions {
    bool declareBothWallHalves;
    bool declareWallOnRead;
    bool setTileBaseColorWhenDistanceCorrect;
    double wheelSpeedFraction;
    InterfaceType interfaceType;
};

struct WorldOptions {
    bool automaticallyClearFog;
    InterfaceType interfaceType;
};

} // namespace sim

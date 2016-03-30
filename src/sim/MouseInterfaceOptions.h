#pragma once

#include "InterfaceType.h"

class IMouseAlgorithm;

namespace sim {

// Some of these options are function pointers, as opposed to primitive values.
// We call the function when we need to determine the value of the options. This
// behavior allows algorithm to change the value of these options on the fly.

struct MouseInterfaceOptions {
    int tileTextNumberOfRows;
    int tileTextNumberOfCols;
    bool (IMouseAlgorithm::*allowOmniscience)(void) const;
    bool (IMouseAlgorithm::*declareBothWallHalves)(void) const;
    bool (IMouseAlgorithm::*setTileTextWhenDistanceDeclared)(void) const;
    bool (IMouseAlgorithm::*setTileBaseColorWhenDistanceDeclaredCorrectly)(void) const;
    bool (IMouseAlgorithm::*declareWallOnRead)(void) const;
    bool (IMouseAlgorithm::*useTileEdgeMovements)(void) const;
    double (IMouseAlgorithm::*wheelSpeedFraction)(void) const;
    InterfaceType interfaceType;
};

} // namespace sim

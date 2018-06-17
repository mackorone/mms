#pragma once

namespace mms {

struct DynamicMouseAlgorithmOptions {
    // Fields and default values
    bool allowOmniscience = false;
    bool declareBothWallHalves = true;  // TODO: MACK - remove
    bool declareWallOnRead = false;     // TODO: MACK - remove
    bool setTileBaseColorWhenDistanceDeclaredCorrectly = false;
    bool setTileTextWhenDistanceDeclared = true; // TODO: MACK - remove
    bool useTileEdgeMovements = false; // TODO: MACK - remove
};

} // namespace mms

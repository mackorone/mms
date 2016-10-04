#pragma once

namespace mms {

struct DynamicMouseAlgorithmOptions {
    // Fields and default values
    bool allowOmniscience = false;
    bool automaticallyClearFog = true;
    bool declareBothWallHalves = true;
    bool declareWallOnRead = false;
    bool setTileBaseColorWhenDistanceDeclaredCorrectly = false;
    bool setTileTextWhenDistanceDeclared = true;
    bool useTileEdgeMovements = false;
};

} // namespace mms

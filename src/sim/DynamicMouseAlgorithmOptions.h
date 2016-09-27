#pragma once

namespace mms {

struct DynamicMouseAlgorithmOptions {
    bool allowOmniscience;
    bool automaticallyClearFog;
    bool declareBothWallHalves;
    bool declareWallOnRead;
    bool setTileBaseColorWhenDistanceDeclaredCorrectly;
    bool setTileTextWhenDistanceDeclared;
    bool useTileEdgeMovements;
};

} // namespace mms

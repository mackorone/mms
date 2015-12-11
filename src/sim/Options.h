#pragma once

namespace sim {

struct Options {
    bool controlTileFog;
    bool declareBothWallHalves;
    bool declareWallOnRead;
    bool setTileBaseColorWhenDistanceCorrect;
    double wheelSpeedFraction;
};

} // namespace sim

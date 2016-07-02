#pragma once

#include <map>

#include "Direction.h"

namespace sim {

struct BasicTile {
    // TODO: MACK - QMap
    std::map<Direction, bool> walls;
};

} // namespace sim

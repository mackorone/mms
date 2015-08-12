#pragma once

#include <map>

#include "Direction.h"

namespace sim {

struct BasicTile {
    int x;
    int y;
    std::map<Direction, bool> walls;
};

} // namespace sim

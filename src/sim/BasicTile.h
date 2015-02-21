#pragma once

#include <map>

#include "Directions.h"

namespace sim {

struct BasicTile {
    int x;
    int y;
    std::map<Direction, bool> walls;
};

} // namespace sim

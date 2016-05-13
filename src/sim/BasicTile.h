#pragma once

#include <map>

#include "Direction.h"

namespace sim {

struct BasicTile {
    std::map<Direction, bool> walls;
};

} // namespace sim

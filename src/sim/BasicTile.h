#pragma once

#include <map>

#include "Direction.h"

namespace sim {

struct BasicTile {
    std::map<Direction, bool> walls;
    BasicTile() {
        for (Direction direction : DIRECTIONS) {
            walls.insert(std::make_pair(direction, false));
        }
    }
};

} // namespace sim

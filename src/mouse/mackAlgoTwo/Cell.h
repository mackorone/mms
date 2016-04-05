#pragma once

#include "../IMouseAlgorithm.h"

namespace mackAlgoTwo {

struct Cell {

    // Properties of the cell
    int x;
    int y;
    bool walls[4];
    bool known[4];

    // Only used for Dijkstra's
    int sequenceNumber;
    Cell* parent;
    int sourceDirection;
    int straightAwayLength;
    float distance;

};

} // namespace mackAlgoTwo

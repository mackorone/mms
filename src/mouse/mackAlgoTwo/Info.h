#pragma once

#include "Direction.h"

namespace mackAlgoTwo {

class Cell;

struct Info {
    unsigned char sequenceNumber;
    float         distance;
    Cell*         parent; // TODO: MACK - write this as a position for one byte
    Direction     sourceDirection; // 2 bits // TODO: MACK - don't need
    unsigned char straightAwayLength; // 4 bits
    unsigned char heapIndex;
};

} // namespace mackAlgoTwo

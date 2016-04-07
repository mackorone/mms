#pragma once

#include "Direction.h"

namespace mackAlgoTwo {

class Cell;

// TODO: MACK - kill direction typedef

struct Info {
    byte sequenceNumber;
    byte parentPosition; // TODO: MACK - write this as a position for one byte
    float         distance;
    Cell*         parent; // TODO: MACK - write this as a position for one byte
    byte sourceDirection; // 2 bits // TODO: MACK - don't need
    byte straightAwayLength; // 4 bits
    byte heapIndex;
};

} // namespace mackAlgoTwo

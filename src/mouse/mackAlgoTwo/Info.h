#pragma once

#include "Byte.h"
#include "Direction.h"

namespace mackAlgoTwo {

struct Info {
    twobyte distance;
    byte sequenceNumber; // 1 bit, boolean
    byte parentIndex; // TODO: MACK - don't need
    byte sourceDirection; // 2 bits // TODO: MACK - don't need
    byte straightAwayLength; // 4 bits
};

} // namespace mackAlgoTwo

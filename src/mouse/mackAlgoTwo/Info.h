#pragma once

#include "Direction.h"

namespace mackAlgoTwo {

struct Info {
    float distance;
    byte  sequenceNumber;
    byte  parentIndex;
    byte  sourceDirection; // 2 bits // TODO: MACK - don't need
    byte  straightAwayLength; // 4 bits
    byte  heapIndex;
};

} // namespace mackAlgoTwo

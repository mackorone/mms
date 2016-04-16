#pragma once

#include "Byte.h"

namespace mackAlgoTwo {

struct Mode {

    // TODO: MACK - I can probs refactor these out
    static const byte EXPLORE_TO_CENTER = 0;
    static const byte EXPLORE_TO_ORIGIN = 1;
    static const byte SOLVE_TO_CENTER   = 2;
    static const byte SOLVE_TO_ORIGIN   = 3;
    static const byte GIVE_UP           = 4;

};

} // namespace mackAlgoTwo

#pragma once

#include "Byte.h"

namespace mackAlgoTwo {

struct Mode {

    static const byte GET_TO_CENTER    = 0;
    static const byte EXPLORE          = 1;
    static const byte RETURN_TO_ORIGIN = 2;
    static const byte SOLVE            = 3;
    static const byte GIVE_UP          = 4;

};

} // namespace mackAlgoTwo

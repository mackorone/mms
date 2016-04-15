#pragma once

#include "Byte.h"

namespace mackAlgoTwo {

class History {

public:

    // Four bits for if we learned anything, four bits of what we learned
    // TODO: MACK
    static void movedTo(byte cell, byte data);
    static void rollBack();

private:

    static const byte CAPACITY = 8;
    static byte m_size;
    static byte m_tail;
    static twobyte m_data[CAPACITY];

};

} // namespace mackAlgoTwo

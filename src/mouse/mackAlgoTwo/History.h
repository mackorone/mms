#pragma once

#include "Byte.h"

namespace mackAlgoTwo {

class History {

public:

    // TODO: MACK
    // Four bits for if we learned anything, four bits of what we learned
    static byte size();
    static void add(byte cell, byte data);
    static void move();
    static twobyte pop();
    static byte cell(twobyte cellAndData);
    static byte data(twobyte cellAndData);

private:

    static const byte CAPACITY = 8;
    static byte m_size;
    static byte m_tail;
    static bool m_infoAdded;

    // wesnwesn TODO: MACK
    static twobyte m_data[CAPACITY];

};

} // namespace mackAlgoTwo

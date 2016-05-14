#pragma once

#include "Byte.h"

namespace mackAlgoTwo {

class History {

    // The History class is a circular stack that remembers the previous
    // CAPACITY moves, and allows you to retrieve the wall information learned
    // by those moves. The proper way to use the History class is to call add()
    // whenever new wall info has been learned, and to call move() whenever the
    // mouse moves from one cell to the next. However, you should call add() at
    // most once between calls to move(), and you should call move() at least
    // once between calls to add(). This is because the History class assumes
    // that, for every cell that the mouse travels, at most one cell's worth of
    // wall information can be learned.

public:

    static byte size();
    static void add(byte cell, byte data);
    static void move();
    static twobyte pop();
    static byte cell(twobyte cellAndData);
    static byte data(twobyte cellAndData);

private:

    // The maximum number of moves/cells remembered by the History class
    static const byte CAPACITY = 16;

    // The total number of moves currently remembered by the History class.
    // It's value is essentially max(CAPACITY, count(move()) - count(pop())).
    static byte m_size;

    // The location in m_data where the information from the next call to add()
    // will be stored. This is incremented by move() and decremented by pop().
    static byte m_tail;

    // Whether or not add() has been called following the most recent call to
    // move(), which helps us to decide whether or not to explicitly overwrite
    // some data the next time that move() is called.
    static bool m_infoAdded;

    // One byte for the x and y position of the cell, and one byte for whether
    // or not we learned of any walls, and what wall values we actually learned
    // (which is technically not needed):
    //
    //                 |---------|---------|---------|---------|
    //            info |    x    |    y    | learned |  walls  |
    //                 |---------|---------|---------|---------|
    //         pos/dir |  00-15  |  00-15  | w s e n | w s e n |
    //                 |---------|---------|---------|---------|
    //            bits | 7 6 5 4 | 3 2 1 0 | 7 6 5 4 | 3 2 1 0 |
    //                 |---------|---------|---------|---------|
    //
    static twobyte m_data[CAPACITY];

};

} // namespace mackAlgoTwo

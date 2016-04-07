#pragma once

#include "Byte.h"

namespace mackAlgoTwo {

struct Maze {

    static const byte WIDTH = 16;
    static const byte HEIGHT = 16;

    // For each cell, we store only eight bits of information: four bits for
    // whether we know the value of a wall, and four bits for the actual value
    //
    //    info |  known  |  walls  |
    //         |---------|---------|
    //     dir | w s e n | w s e n |
    //         |---------|---------|
    //    bits | 7 6 5 4 | 3 2 1 0 |
    //
    // Furthermore, wach cell can be indexed by just eight bits:
    // four bits for the x position, and four bits for the y position
    //
    //    axes |    x    |    y    |
    //         |---------|---------|
    //  values |  00-15  |  00-15  |
    //         |---------|---------|
    //    bits | 7 6 5 4 | 3 2 1 0 |
    //      
    static byte data[WIDTH * HEIGHT];

    static byte getX(byte index);
    static byte getY(byte index);
    static byte getIndex(byte x, byte y);

    static bool isKnown(byte index, byte direction);
    static bool isWall(byte index, byte direction);
    static void setWall(byte index, byte direction, bool isWall);

    static bool isKnown(byte x, byte y, byte direction);
    static bool isWall(byte x, byte y, byte direction);
    static void setWall(byte x, byte y, byte direction, bool isWall);

    // Return the index of the cell at the lower left and
    // upper right, respectively, of the center of the maze
    static byte getCenterLowerLeftIndex();
    static byte getCenterUpperRightIndex();

};

} // namespace mackAlgoTwo

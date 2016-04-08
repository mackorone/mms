#pragma once

#include "Byte.h"

// TODO: MACK
#include "Info.h"

namespace mackAlgoTwo {

class Maze {

public:

    // The width and height of the maze, as understood
    // by the algorithm, both of which must be in [1, 16]
    static const byte WIDTH  = 16;
    static const byte HEIGHT = 16;

    // The x and y positions of the lower left and upper right center cells
    static const byte CLLX = (WIDTH  - 1) / 2;
    static const byte CLLY = (HEIGHT - 1) / 2;
    static const byte CURX = (WIDTH     ) / 2;
    static const byte CURY = (HEIGHT    ) / 2;

    // Helper methods for converting between xy coordinates
    // and the maze index of the cell in the data array
    static byte getX(byte mazeIndex);
    static byte getY(byte mazeIndex);
    static byte getIndex(byte x, byte y);

    // Helper methods for querying and updating maze data
    static bool isKnown(byte x, byte y, byte direction);
    static bool isWall(byte x, byte y, byte direction);
    static void setWall(byte x, byte y, byte direction, bool isWall);
    static bool isKnown(byte mazeIndex, byte direction);
    static bool isWall(byte mazeIndex, byte direction);
    static void setWall(byte mazeIndex, byte direction, bool isWall);

    // TODO: MACK
    static Info info[WIDTH * HEIGHT];

private:

    // For each cell, we store only eight bits of information: four bits for
    // whether we know the value of a wall, and four bits for the actual value
    //
    //    info |  known  |  walls  |
    //         |---------|---------|
    //     dir | w s e n | w s e n |
    //         |---------|---------|
    //    bits | 7 6 5 4 | 3 2 1 0 |
    //
    // Furthermore, each cell can be indexed by just eight bits: at most
    // four bits for the x position, and at most four bits for the y position
    //      
    static byte m_data[WIDTH * HEIGHT];

};

} // namespace mackAlgoTwo

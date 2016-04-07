#pragma once

#include "Byte.h"

namespace mackAlgoTwo {

struct Maze {

    // The width and height of the maze, as understood
    // by the algorithm, both of which must be in [1, 16]
    static const byte WIDTH = 16;
    static const byte HEIGHT = 16;

    // The x and y positions of the lower left and upper right center cells
    static const byte CLLX = (WIDTH - 1) / 2;
    static const byte CLLY = (HEIGHT - 1) / 2;
    static const byte CURX = WIDTH / 2;
    static const byte CURY = HEIGHT / 2;

    // For each cell, we store only eight bits of information: four bits for
    // whether we know the value of a wall, and four bits for the actual value
    //
    //    info |  known  |  walls  |
    //         |---------|---------|
    //     dir | w s e n | w s e n |
    //         |---------|---------|
    //    bits | 7 6 5 4 | 3 2 1 0 |
    //
    // Furthermore, each cell can be indexed by just eight bits:
    // four bits for the x position, and four bits for the y position
    //
    //    axes |    x    |    y    |
    //         |---------|---------|
    //  values |  00-15  |  00-15  |
    //         |---------|---------|
    //    bits | 7 6 5 4 | 3 2 1 0 |
    //      
    // TODO: MACK - this is broken for mazes that aren't 16 x 16
    static byte data[WIDTH * HEIGHT];

    // Helper methods for converting between xy coordinates
    // and the index of the cell in the data array
    static byte getX(byte index);
    static byte getY(byte index);
    static byte getIndex(byte x, byte y);

    // Helper methods for querying and updating maze data
    static bool isKnown(byte index, byte direction);
    static bool isWall(byte index, byte direction);
    static void setWall(byte index, byte direction, bool isWall);
    static bool isKnown(byte x, byte y, byte direction);
    static bool isWall(byte x, byte y, byte direction);
    static void setWall(byte x, byte y, byte direction, bool isWall);

};

} // namespace mackAlgoTwo

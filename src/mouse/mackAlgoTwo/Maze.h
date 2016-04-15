#pragma once

#include "Byte.h"
#include "Direction.h"

namespace mackAlgoTwo {

struct Info {
    // The distance of the cell from the source (no units)
    twobyte distance;
    // bit 0 is whether or not the cell has been discovered
    // bit 1 is whether or not the cell has a "next" cell
    // bits 2 - 3 are the direction of the "next" cell
    // bits 4 - 7 are the straightaway length
    byte misc;
};

struct Maze {

    // The width and height of the maze, as understood
    // by the algorithm, both of which must be in [1, 16]
    static const byte WIDTH  = 16;
    static const byte HEIGHT = 16;

    // The x and y positions of the lower left and upper right center cells
    static const byte CLLX = (WIDTH  - 1) / 2;
    static const byte CLLY = (HEIGHT - 1) / 2;
    static const byte CURX = (WIDTH     ) / 2;
    static const byte CURY = (HEIGHT    ) / 2;

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

    // Helper methods for converting between xy coordinates
    // and the maze index of the cell in the data array
    static byte getX(byte cell);
    static byte getY(byte cell);
    static byte getCell(byte x, byte y);

    // Helper methods for querying and updating maze data
    static bool isKnown(byte x, byte y, byte direction);
    static bool isWall(byte x, byte y, byte direction);
    static void setWall(byte x, byte y, byte direction, bool isWall);
    static void unsetWall(byte x, byte y, byte direction);
    static bool isKnown(byte cell, byte direction);
    static bool isWall(byte cell, byte direction);
    static void setWall(byte cell, byte direction, bool isWall);
    static void unsetWall(byte cell, byte direction);

    // Information used only by Dijkstra's algo to determine the fastest path
    static Info m_info[WIDTH * HEIGHT];

    // Helper methods for accessing and modifying m_info
    static twobyte getDistance(byte cell);
    static void setDistance(byte cell, twobyte distance);
    static bool getDiscovered(byte cell);
    static void setDiscovered(byte cell, bool discovered);
    static bool hasNext(byte cell);
    static void clearNext(byte cell);
    static byte getNextDirection(byte cell);
    static void setNextDirection(byte cell, byte nextDirection);
    static byte getStraightAwayLength(byte cell);
    static void setStraightAwayLength(byte cell, byte straightAwayLength);

};

} // namespace mackAlgoTwo

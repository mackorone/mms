#pragma once

#include "Direction.h"
#include "Info.h"

namespace mackAlgoTwo {

class Cell {

public:

    Cell();

    void init(
        unsigned char x,
        unsigned char y,
        unsigned char mazeWidth,
        unsigned char mazeHeight);

    unsigned char getX() const;
    unsigned char getY() const;
    unsigned char getPosition() const;

    bool isKnown(unsigned char direction) const;
    bool isWall(unsigned char direction) const;
    void setWall(unsigned char direction, bool isWall);

    // Any extra temporary info needed by Dijkstra's
    Info info;

private:

    // Four bits for each of x and y
    //
    //   axes |    x    |    y    |
    //        |---------|---------|
    // values |  00-15  |  00-15  |
    //        |---------|---------|
    //   bits | 7 6 5 4 | 3 2 1 0 |
    //
    unsigned char m_position;

    // Four bits for whether we know
    // the value of the wall, and four
    // bits for the value of the wall
    //
    //   info |  known  |  walls  |
    //        |---------|---------|
    //    dir | w s e n | w s e n |
    //        |---------|---------|
    //   bits | 7 6 5 4 | 3 2 1 0 |
    //
    unsigned char m_walls;
};

} // namespace mackAlgoTwo

#pragma once

#include "Direction.h"
#include "Info.h"

namespace mackAlgoTwo {

class Cell {

public:

    Cell();

    void init(
        byte x,
        byte y,
        byte mazeWidth,
        byte mazeHeight);

    byte getX() const;
    byte getY() const;
    byte getPosition() const;

    bool isKnown(byte direction) const;
    bool isWall(byte direction) const;
    void setWall(byte direction, bool isWall);

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
    byte m_position;

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
    byte m_walls;
};

} // namespace mackAlgoTwo

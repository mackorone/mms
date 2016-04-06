#pragma once

#include "Info.h"

namespace mackAlgoTwo {

// TODO: MACK - move this to it's own file
enum {
    NORTH = 0,
    EAST  = 1,
    SOUTH = 2,
    WEST  = 3,
};

class Cell {

public:
    Cell();

    void init(int x, int y, int mazeWidth, int mazeHeight);

    int getX() const;
    int getY() const;

    bool isKnown(int direction) const;
    bool isWall(int direction) const;
    void setWall(int direction, bool isWall);

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

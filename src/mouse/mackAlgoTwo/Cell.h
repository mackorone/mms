#pragma once

#include "../IMouseAlgorithm.h"

namespace mackAlgoTwo {

enum {
    NORTH = 0,
    EAST  = 1,
    SOUTH = 2,
    WEST  = 3,
};

class Cell {

public:
    Cell();
    void init(
        sim::MouseInterface* mouse,
        int x,
        int y,
        int mazeWidth,
        int mazeHeight);

    int getX() const;
    int getY() const;

    bool isKnown(int direction) const;
    bool isWall(int direction) const;
    void setWall(int direction, bool isWall);

    // ----- Used for Dijkstra's ----- //

    int getSequenceNumber() const;
    void setSequenceNumber(int sequenceNumber);
    
    Cell* getParent() const;
    void setParent(Cell* parent);

    int getSourceDirection() const;
    void setSourceDirection(int sourceDirection);

    int getStraightAwayLength() const;
    void setStraightAwayLength(int straightAwayLength);

    float getDistance() const;
    void setDistance(float distance);

    int getHeapIndex() const;
    void setHeapIndex(int index);

private:

    // The interface through which we control the mouse
    sim::MouseInterface* m_mouse;

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

    // Used for Dijkstra's
    // TODO: MACK - separate struct for this
    int m_sequenceNumber;
    Cell* m_parent;
    int m_sourceDirection;
    int m_straightAwayLength;
    float m_distance;
    int m_heapIndex;
};

} // namespace mackAlgoTwo

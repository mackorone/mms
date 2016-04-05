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

    char getColor() const;
    void setColor(char color);

private:
    sim::MouseInterface* m_mouse;
    int m_x;
    int m_y;
    bool m_walls[4];
    bool m_known[4];

    // Used for Dijkstra's
    int m_sequenceNumber;
    Cell* m_parent;
    int m_sourceDirection;
    int m_straightAwayLength;
    float m_distance;
    int m_heapIndex;

    // TODO: MACK
    char m_color;
};

} // namespace mackAlgoTwo

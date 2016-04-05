#pragma once

#include "../IMouseAlgorithm.h"

namespace mackAlgoTwo {

// TODO: MACK - turn this into a struct
class Cell {

public:
    Cell();

    void setMouseInterface(sim::MouseInterface* mouse);

    int getX() const;
    int getY() const;
    void setPosition(int x, int y);

    bool isWall(int direction) const;
    void setWall(int direction, bool isWall);

    bool isKnown(int direction) const;
    void setKnown(int direction, bool isKnown);

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
    sim::MouseInterface* m_mouse;
    int m_x; // TODO: MACK - turn these into chars
    int m_y;
    bool m_walls[4]; // TODO: MACK - turn these into bytes
    bool m_known[4];

    // Used for Dijkstra's
    int m_sequenceNumber;
    Cell* m_parent;
    int m_sourceDirection;
    int m_straightAwayLength;
    float m_distance;

    // TODO: MACK
    int m_heapIndex;

};

} // namespace mackAlgoTwo

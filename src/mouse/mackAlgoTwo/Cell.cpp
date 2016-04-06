#include "Cell.h"

namespace mackAlgoTwo {

Cell::Cell() :
        m_mouse(nullptr),
        m_position(0),
        m_walls(0),
        m_sequenceNumber(-1),
        m_parent(nullptr),
        m_sourceDirection(-1),
        m_straightAwayLength(0),
        m_distance(-1),
        m_heapIndex(-1) {
}

void Cell::init(
        sim::MouseInterface* mouse,
        int x,
        int y,
        int mazeWidth,
        int mazeHeight) {
    // TODO: MACK - only works for 16 by 16
    m_mouse = mouse;
    m_position = x << 4 | y;
    if (x == 0) {
        setWall(WEST, true);
    }
    if (y == 0) {
        setWall(SOUTH, true);
    }
    if (x == mazeWidth - 1) {
        setWall(EAST, true);
    }
    if (y == mazeHeight - 1) {
        setWall(NORTH, true);
    }
}

int Cell::getX() const {
    return m_position >> 4;
}

int Cell::getY() const {
    return m_position & 15;
}

bool Cell::isKnown(int direction) const {
    // TODO: MACK +4 make a note of this
    return (m_walls >> direction + 4) & 1;
}

bool Cell::isWall(int direction) const {
    return (m_walls >> direction) & 1;
}

void Cell::setWall(int direction, bool isWall) {
    static char directionChars[] = {'n', 'e', 's', 'w'};
    if (isWall) {
        m_walls |=  (1 << direction);
    }
    else {
        m_walls &= ~(1 << direction);
    }
    m_walls |= 1 << direction + 4;
    m_mouse->declareWall(getX(), getY(), directionChars[direction], isWall);
}

int Cell::getSequenceNumber() const {
    return m_sequenceNumber;
}

void Cell::setSequenceNumber(int sequenceNumber) {
    m_sequenceNumber = sequenceNumber;
}

Cell* Cell::getParent() const {
    return m_parent;
}

void Cell::setParent(Cell* parent) {
    m_parent = parent;
}

int Cell::getSourceDirection() const {
    return m_sourceDirection;
}

void Cell::setSourceDirection(int sourceDirection) {
    m_sourceDirection = sourceDirection;
}

int Cell::getStraightAwayLength() const {
    return m_straightAwayLength;
}

void Cell::setStraightAwayLength(int straightAwayLength) {
    m_straightAwayLength = straightAwayLength;
}

float Cell::getDistance() const {
    return m_distance;
}

void Cell::setDistance(float distance) {
    m_distance = distance;
    m_mouse->setTileText(getX(), getY(), std::to_string(distance));
}

int Cell::getHeapIndex() const {
    return m_heapIndex;
}

void Cell::setHeapIndex(int index) {
    m_heapIndex = index;
}

} // namespace mackAlgoTwo

#include "Cell.h"

namespace mackAlgoTwo {

Cell::Cell() :
        m_mouse(nullptr),
        m_x(-1),
        m_y(-1),
        m_sequenceNumber(-1),
        m_parent(nullptr),
        m_sourceDirection(-1),
        m_straightAwayLength(0),
        m_distance(-1),
        m_heapIndex(-1),
        m_color('r') {
    for (int i = 0; i < 4; i += 1) {
        m_walls[i] = false;
        m_known[i] = false;
    }
}

void Cell::init(
        sim::MouseInterface* mouse,
        int x,
        int y,
        int mazeWidth,
        int mazeHeight) {
    m_mouse = mouse;
    m_x = x;
    m_y = y;
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
    return m_x;
}

int Cell::getY() const {
    return m_y;
}

bool Cell::isKnown(int direction) const {
    return m_known[direction];
}

bool Cell::isWall(int direction) const {
    return m_walls[direction];
}

void Cell::setWall(int direction, bool isWall) {
    static char directionChars[] = {'n', 'e', 's', 'w'};
    m_walls[direction] = isWall;
    m_known[direction] = true;
    m_mouse->declareWall(m_x, m_y, directionChars[direction], isWall);
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
    m_mouse->setTileText(m_x, m_y, std::to_string(distance));
}

int Cell::getHeapIndex() const {
    return m_heapIndex;
}

void Cell::setHeapIndex(int index) {
    m_heapIndex = index;
}

char Cell::getColor() const {
    return m_color;
}

void Cell::setColor(char color) {
    m_color = color;
}

} // namespace mackAlgoTwo

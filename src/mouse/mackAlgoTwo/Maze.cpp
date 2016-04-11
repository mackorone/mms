#include "Maze.h"

namespace mackAlgoTwo {

byte Maze::m_data[] = {0};
Info Maze::m_info[] = {0};

byte Maze::getX(byte cell) {
    return cell / HEIGHT;
}

byte Maze::getY(byte cell) {
    return cell % HEIGHT;
}

byte Maze::getCell(byte x, byte y) {
    return x * HEIGHT + y;
}

bool Maze::isKnown(byte x, byte y, byte direction) {
    return isKnown(getCell(x, y), direction);
}

bool Maze::isWall(byte x, byte y, byte direction) {
    return isWall(getCell(x, y), direction);
}

void Maze::setWall(byte x, byte y, byte direction, bool isWall) {
    setWall(getCell(x, y), direction, isWall);
}

bool Maze::isKnown(byte cell, byte direction) {
    return (m_data[cell] >> direction + 4) & 1;
}

bool Maze::isWall(byte cell, byte direction) {
    return (m_data[cell] >> direction) & 1;
}

void Maze::setWall(byte cell, byte direction, bool isWall) {
    m_data[cell] |= 1 << direction + 4;
    m_data[cell] =
        (m_data[cell] & ~(1 << direction)) | (isWall ? 1 << direction : 0);
}

twobyte Maze::getDistance(byte cell) {
    return m_info[cell].distance;
}

void Maze::setDistance(byte cell, twobyte distance) {
    m_info[cell].distance = distance;
}

bool Maze::getSequenceBit(byte cell) {
    return m_info[cell].data & 1;
}

void Maze::flipSequenceBit(byte cell) {
    m_info[cell].data ^= 1;
}

bool Maze::getHasParent(byte cell) {
    return m_info[cell].data & 2;
}

void Maze::setHasParent(byte cell, bool hasParent) {
    m_info[cell].data = (m_info[cell].data & ~2) | (hasParent ? 2 : 0);
}

byte Maze::getSourceDirection(byte cell) {
    return m_info[cell].data >> 2 & 3;
}

void Maze::setSourceDirection(byte cell, byte sourceDirection) {
    m_info[cell].data = (m_info[cell].data & ~12) | (sourceDirection << 2);
}

byte Maze::getStraightAwayLength(byte cell) {
    return m_info[cell].data >> 4 & 15;
}

void Maze::setStraightAwayLength(byte cell, byte straightAwayLength) {
    m_info[cell].data = (m_info[cell].data & 15) | (straightAwayLength << 4);
}

byte Maze::getParentIndex(byte cell) {
    return m_info[cell].parentIndex;
}

void Maze::setParentIndex(byte cell, byte parentIndex) {
    m_info[cell].parentIndex = parentIndex;
}

} // namespace mackAlgoTwo

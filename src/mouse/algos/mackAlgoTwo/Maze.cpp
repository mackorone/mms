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

void Maze::unsetWall(byte x, byte y, byte direction) {
    unsetWall(getCell(x, y), direction);
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

void Maze::unsetWall(byte cell, byte direction) {
    m_data[cell] &= ~(1 << direction + 4);
    m_data[cell] &= ~(1 << direction);
}

twobyte Maze::getDistance(byte cell) {
    return m_info[cell].distance;
}

void Maze::setDistance(byte cell, twobyte distance) {
    m_info[cell].distance = distance;
}

bool Maze::getDiscovered(byte cell) {
    return m_info[cell].misc & 1;
}

void Maze::setDiscovered(byte cell, bool discovered) {
    m_info[cell].misc = (m_info[cell].misc & ~1) | (discovered ? 1 : 0);
}

bool Maze::hasNext(byte cell) {
    return m_info[cell].misc & 2;
}

void Maze::clearNext(byte cell) {
    m_info[cell].misc &= ~2;
}

byte Maze::getNextDirection(byte cell) {
    return m_info[cell].misc >> 2 & 3;
}

void Maze::setNextDirection(byte cell, byte nextDirection) {
    m_info[cell].misc |= 2;
    m_info[cell].misc = (m_info[cell].misc & ~12) | (nextDirection << 2);
}

byte Maze::getStraightAwayLength(byte cell) {
    return m_info[cell].misc >> 4 & 15;
}

void Maze::setStraightAwayLength(byte cell, byte straightAwayLength) {
    m_info[cell].misc = (m_info[cell].misc & 15) | (straightAwayLength << 4);
}

} // namespace mackAlgoTwo

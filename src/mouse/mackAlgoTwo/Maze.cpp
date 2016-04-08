#include "Maze.h"

namespace mackAlgoTwo {

byte Maze::m_data[];
Info Maze::info[];

byte Maze::getX(byte mazeIndex) {
    return mazeIndex / HEIGHT;
}

byte Maze::getY(byte mazeIndex) {
    return mazeIndex % HEIGHT;
}

byte Maze::getIndex(byte x, byte y) {
    return x * HEIGHT + y;
}

bool Maze::isKnown(byte x, byte y, byte direction) {
    return isKnown(getIndex(x, y), direction);
}

bool Maze::isWall(byte x, byte y, byte direction) {
    return isWall(getIndex(x, y), direction);
}

void Maze::setWall(byte x, byte y, byte direction, bool isWall) {
    setWall(getIndex(x, y), direction, isWall);
}

bool Maze::isKnown(byte mazeIndex, byte direction) {
    return (m_data[mazeIndex] >> direction + 4) & 1;
}

bool Maze::isWall(byte mazeIndex, byte direction) {
    return (m_data[mazeIndex] >> direction) & 1;
}

void Maze::setWall(byte mazeIndex, byte direction, bool isWall) {
    if (isWall) {
        m_data[mazeIndex] |= (1 << direction);
    }
    else {
        m_data[mazeIndex] &= ~(1 << direction);
    }
    m_data[mazeIndex] |= 1 << direction + 4;
}

} // namespace mackAlgoTwo

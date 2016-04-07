#include "Maze.h"

namespace mackAlgoTwo {

byte Maze::data[];

byte Maze::getX(byte index) {
    return index >> 4;
}

byte Maze::getY(byte index) {
    return index & 15;
}

byte Maze::getIndex(byte x, byte y) {
    return x << 4 | y;
}

bool Maze::isKnown(byte index, byte direction) {
    return (data[index] >> direction + 4) & 1;
}

bool Maze::isWall(byte index, byte direction) {
    return (data[index] >> direction) & 1;
}

void Maze::setWall(byte index, byte direction, bool isWall) {
    if (isWall) {
        data[index] |= (1 << direction);
    }
    else {
        data[index] &= ~(1 << direction);
    }
    data[index] |= 1 << direction + 4;
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

} // namespace mackAlgoTwo

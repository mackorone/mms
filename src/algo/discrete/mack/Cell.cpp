#include "Cell.h"

namespace mack {

Cell::Cell() : m_x(-1), m_y(-1) {
    for (int i = 0; i < 4; i += 1) {
        m_walls[i] = false;
    }
}

int Cell::getX() const {
    return m_x;
}

int Cell::getY() const {
    return m_y;
}

void Cell::setPosition(int x, int y) {
    m_x = x;
    m_y = y;
}

bool Cell::isWall(int direction) const {
    return m_walls[direction];
}

void Cell::setWall(int direction, bool isWall) {
    m_walls[direction] = isWall;
}

} // namespace mack

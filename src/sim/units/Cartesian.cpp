#include "Cartesian.h"

namespace sim {

Cartesian::Cartesian(float x, float y) {
    m_x = x;
    m_y = y;
}

Cartesian::Cartesian(const Coordinate& coordinate) {
    m_x = coordinate.getX();
    m_y = coordinate.getY();
}

Cartesian Cartesian::operator+(const Coordinate& coordinate) const {
    return Cartesian(getX() + coordinate.getX(), getY() + coordinate.getY());
}

} // namespace sim

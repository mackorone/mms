#include "Cartesian.h"

namespace sim {

Cartesian::Cartesian(const Distance& x, const Distance& y) {
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

void Cartesian::operator+=(const Coordinate& coordinate) {
    m_x += coordinate.getX();
    m_y += coordinate.getY();
}

} // namespace sim

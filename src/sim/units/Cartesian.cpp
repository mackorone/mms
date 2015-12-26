#include "Cartesian.h"

namespace sim {

Cartesian::Cartesian() {
    m_x = 0.0;
    m_y = 0.0;
}

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

Cartesian Cartesian::operator-(const Coordinate& coordinate) const {
    return Cartesian(getX() - coordinate.getX(), getY() - coordinate.getY());
}

Cartesian Cartesian::operator*(double factor) const {
    return Cartesian(getX() * factor, getY() * factor);
}

Cartesian Cartesian::operator/(double factor) const {
    return Cartesian(getX() / factor, getY() / factor);
}

void Cartesian::operator+=(const Coordinate& coordinate) {
    m_x += coordinate.getX();
    m_y += coordinate.getY();
}

} // namespace sim

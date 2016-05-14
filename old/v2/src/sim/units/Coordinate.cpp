#include "Coordinate.h"

#include "../CPMath.h"

namespace sim {

Coordinate::Coordinate() : m_x(Meters(0.0)), m_y(Meters(0.0)) {
}

Coordinate::~Coordinate() {
}

Meters Coordinate::getX() const {
    return m_x;
}

Meters Coordinate::getY() const {
    return m_y;
}

Meters Coordinate::getRho() const {
    return Meters(std::hypot(m_x.getMeters(), m_y.getMeters()));
}

Radians Coordinate::getTheta() const {
    return Radians(std::atan2(m_y.getMeters(), m_x.getMeters()));
}

bool Coordinate::operator==(const Coordinate& coordinate) const {
    return (getX() == coordinate.getX() && getY() == coordinate.getY());
}

bool Coordinate::operator!=(const Coordinate& coordinate) const {
    return (!(operator==(coordinate)));
}

bool Coordinate::operator<(const Coordinate& coordinate) const {
    return (getX() != coordinate.getX() ? getX() < coordinate.getX() : getY() < coordinate.getY());
}

} // namespace sim

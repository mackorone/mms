#include "Coordinate.h"

#include <cmath>

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
    return Meters(hypot(m_x.getMeters(), m_y.getMeters()));
}

Radians Coordinate::getTheta() const {
    return Radians(atan2(m_y.getMeters(), m_x.getMeters()));
}

bool Coordinate::operator==(const Coordinate& coordinate) {
    return (getX() == coordinate.getX() && getY() == coordinate.getY());
}

} // namespace sim

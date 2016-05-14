#include "Polar.h"

#include "Cartesian.h"

namespace sim {

Polar::Polar(const Distance& rho, const Angle& theta) {
    m_x = Meters(rho.getMeters() * theta.getCos());
    m_y = Meters(rho.getMeters() * theta.getSin());
}

Polar::Polar(const Coordinate& coordinate) {
    m_x = coordinate.getX();
    m_y = coordinate.getY();
}

Polar Polar::operator+(const Coordinate& coordinate) const {
    return Polar(Cartesian(getX() + coordinate.getX(), getY() + coordinate.getY()));
}

} // namespace sim

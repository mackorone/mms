#include "Polar.h"

#include <cmath>

namespace sim {

Polar::Polar(const Distance& rho, const Angle& theta) {
    m_x = Meters(rho.getMeters() * cos(theta.getRadians()));
    m_y = Meters(rho.getMeters() * sin(theta.getRadians()));
}

Polar::Polar(const Coordinate& coordinate) {
    m_x = coordinate.getX();
    m_y = coordinate.getY();
}

Polar Polar::operator+(const Coordinate& coordinate) const {
    return Polar(getRho() + coordinate.getRho(), getTheta() + coordinate.getTheta());
}

} // namespace sim

#include "Polar.h"

#include <cmath>

namespace sim {

Polar::Polar(float rho, float theta) {
    m_x = rho*cos(theta);
    m_y = rho*sin(theta);
}

Polar::Polar(const Coordinate& coordinate) {
    m_x = coordinate.getX();
    m_y = coordinate.getY();
}

Polar Polar::operator+(const Coordinate& coordinate) const {
    return Polar(getRho() + coordinate.getRho(), getTheta() + coordinate.getTheta());
}

} // namespace sim

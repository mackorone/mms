#include "Degrees.h"

#include <cmath>

namespace sim {

Degrees::Degrees(float degrees) {
    m_radians = degrees * (2 * M_PI) / 360;
}

Degrees::Degrees(const Angle& angle) {
    m_radians = angle.getRadians();
}

Degrees Degrees::operator+(const Angle& angle) const {
    return Degrees(getDegrees() + angle.getDegrees());
}

} // namespace sim

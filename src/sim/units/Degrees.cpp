#include "Degrees.h"

#include <cmath>

namespace sim {

Degrees::Degrees() {
    m_radians = 0.0;
}

Degrees::Degrees(double degrees) {
    m_radians = degrees * (2 * M_PI) / 360;
}

Degrees::Degrees(const Angle& angle) {
    m_radians = angle.getRadians();
}

Degrees Degrees::operator+(const Angle& angle) const {
    return Degrees(getDegrees() + angle.getDegrees());
}

Degrees Degrees::operator-(const Angle& angle) const {
    return Degrees(getDegrees() - angle.getDegrees());
}

Degrees Degrees::operator*(double factor) const {
    return Degrees(getDegrees() * factor);
}

} // namespace sim

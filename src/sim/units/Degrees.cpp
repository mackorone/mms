#include "Degrees.h"

#include "../CPMath.h"

namespace sim {

Degrees::Degrees() {
    m_radians = 0.0;
}

Degrees::Degrees(double degrees) {
    static const double radiansPerDegree = M_TWOPI / 360.0;
    m_radians = radiansPerDegree * degrees;
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

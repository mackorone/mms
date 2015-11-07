#include "Angle.h"

#include "../Assert.h"
#include "../CPMath.h"

namespace sim {

Angle::Angle() : m_radians(0) {
}

Angle::~Angle() {
}

double Angle::getRadians() const {
    // Ensure [0, 2pi)
    double radians = m_radians - M_TWOPI * floor(m_radians / M_TWOPI);
    ASSERT_LE(0, radians);
    ASSERT_LT(radians, M_TWOPI);
    return radians;
}

double Angle::getDegrees() const {
    return getRadians() * 360 / (2 * M_PI);
}

double Angle::getSin() const {
    return sin(getRadians());
}

double Angle::getCos() const {
    return cos(getRadians());
}

bool Angle::operator<(const Angle& angle) {
    return getRadians() < angle.getRadians();
}

} // namespace sim

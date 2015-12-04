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
    double radians = fmod(m_radians, M_TWOPI);
    if (radians < 0) {
       radians += M_TWOPI;
    }
    if (M_TWOPI <= radians) {
        radians -= M_TWOPI;
    }
    ASSERT_LE(0, radians);
    ASSERT_LT(radians, M_TWOPI);
    return radians;
}

double Angle::getDegrees() const {
    static const double degreesPerRadian = 360.0 / M_TWOPI;
    return degreesPerRadian * getRadians();
}

double Angle::getSin() const {
    return std::sin(getRadians());
}

double Angle::getCos() const {
    return std::cos(getRadians());
}

bool Angle::operator<(const Angle& angle) {
    return getRadians() < angle.getRadians();
}

} // namespace sim

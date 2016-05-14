#include "Angle.h"

#include "../Assert.h"
#include "../CPMath.h"

namespace sim {

Angle::Angle() : m_radians(0) {
}

Angle::~Angle() {
}

double Angle::getRadiansZeroTo2pi() const {
    return getRadians(true);
}

double Angle::getDegreesZeroTo360() const {
    return getDegrees(true);
}

double Angle::getRadiansNotBounded() const {
    return getRadians(false);
}

double Angle::getDegreesNotBounded() const {
    return getDegrees(false);
}

double Angle::getSin() const {
    return std::sin(getRadiansZeroTo2pi());
}

double Angle::getCos() const {
    return std::cos(getRadiansZeroTo2pi());
}

bool Angle::operator<(const Angle& angle) const {
    return getRadiansZeroTo2pi() < angle.getRadiansZeroTo2pi();
}

double Angle::getRadians(bool zeroTo2pi) const {
    double radians = m_radians;
    if (zeroTo2pi) {
        radians = std::fmod(radians, M_TWOPI);
        if (radians < 0) {
           radians += M_TWOPI;
        }
        if (M_TWOPI <= radians) {
            radians -= M_TWOPI;
        }
        SIM_ASSERT_LE(0, radians);
        SIM_ASSERT_LT(radians, M_TWOPI);
    }
    return radians;
}

double Angle::getDegrees(bool zeroTo360) const {
    static const double degreesPerRadian = 360.0 / M_TWOPI;
    return degreesPerRadian * getRadians(zeroTo360);
}

} // namespace sim

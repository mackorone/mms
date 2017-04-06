#include "Angle.h"

#include <QtMath>

#include "../Assert.h"

namespace mms {

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
        radians = std::fmod(radians, 2 * M_PI);
        if (radians < 0) {
           radians += 2 * M_PI;
        }
        if (2 * M_PI <= radians) {
            radians -= 2 * M_PI;
        }
        ASSERT_LE(0, radians);
        ASSERT_LT(radians, 2 * M_PI);
    }
    return radians;
}

double Angle::getDegrees(bool zeroTo360) const {
    static const double degreesPerRadian = 360.0 / (2 * M_PI);
    return degreesPerRadian * getRadians(zeroTo360);
}

} // namespace mms

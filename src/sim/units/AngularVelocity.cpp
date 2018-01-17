#include "AngularVelocity.h"

#include <QtMath>

namespace mms {

AngularVelocity::AngularVelocity() : m_radiansPerSecond(0) {
}

AngularVelocity::~AngularVelocity() {
}

double AngularVelocity::getRadiansPerSecond() const {
    return m_radiansPerSecond;
}

double AngularVelocity::getDegreesPerSecond() const {
    static const double degreesPerRadian = 360.0 / (2 * M_PI);
    return degreesPerRadian * getRadiansPerSecond();
}

double AngularVelocity::getRevolutionsPerMinute() const {
    static const double revolutionsPerRadian = 1.0 / (2 * M_PI);
    static const double secondsPerMinute = 60.0;
    return revolutionsPerRadian * getRadiansPerSecond() * secondsPerMinute;
}

Angle AngularVelocity::operator*(const Duration& duration) const {
    return Angle::Radians(getRadiansPerSecond() * duration.getSeconds());
}

double AngularVelocity::operator/(const AngularVelocity& angularVelocity) const {
    return getRadiansPerSecond() / angularVelocity.getRadiansPerSecond();
}

bool AngularVelocity::operator<(const AngularVelocity& angularVelocity) const {
    return getRadiansPerSecond() < angularVelocity.getRadiansPerSecond();
}

bool AngularVelocity::operator<=(const AngularVelocity& angularVelocity) const {
    return getRadiansPerSecond() <= angularVelocity.getRadiansPerSecond();
}

} // namespace mms

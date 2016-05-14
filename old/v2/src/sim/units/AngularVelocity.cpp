#include "AngularVelocity.h"

#include "../CPMath.h"

namespace sim {

AngularVelocity::AngularVelocity() : m_radiansPerSecond(0) {
}

AngularVelocity::~AngularVelocity() {
}

double AngularVelocity::getRadiansPerSecond() const {
    return m_radiansPerSecond;
}

double AngularVelocity::getDegreesPerSecond() const {
    static const double degreesPerRadian = 360.0 / M_TWOPI;
    return degreesPerRadian * getRadiansPerSecond();
}

double AngularVelocity::getRevolutionsPerMinute() const {
    static const double revolutionsPerRadian = 1.0 / M_TWOPI;
    static const double secondsPerMinute = 60.0;
    return revolutionsPerRadian * getRadiansPerSecond() * secondsPerMinute;
}

Radians AngularVelocity::operator*(const Duration& duration) const {
    return Radians(getRadiansPerSecond() * duration.getSeconds());
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

} // namespace sim

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
    return getRadiansPerSecond() * 180.0 / M_PI;
}

double AngularVelocity::getRevolutionsPerSecond() const {
    return getRadiansPerSecond() / M_TWOPI;
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

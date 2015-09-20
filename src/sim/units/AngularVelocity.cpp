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
    return m_radiansPerSecond * 180.0 / M_PI;
}

Radians AngularVelocity::operator*(const Duration& duration) const {
    return m_radiansPerSecond * duration.getSeconds();
}

} // namespace sim

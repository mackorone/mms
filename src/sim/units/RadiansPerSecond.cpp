#include "RadiansPerSecond.h"

namespace sim {

RadiansPerSecond::RadiansPerSecond(double radiansPerSecond) {
    m_radiansPerSecond = radiansPerSecond;
}

RadiansPerSecond::RadiansPerSecond(const AngularVelocity& angularVelocity) {
    m_radiansPerSecond = angularVelocity.getRadiansPerSecond();
}

RadiansPerSecond RadiansPerSecond::operator+(const AngularVelocity& angularVelocity) const {
    return RadiansPerSecond(getRadiansPerSecond() + angularVelocity.getRadiansPerSecond());
}

RadiansPerSecond RadiansPerSecond::operator/(double factor) const {
    return RadiansPerSecond(getRadiansPerSecond() / factor);
}

Radians RadiansPerSecond::operator*(const Duration& duration) const {
    return Radians(getRadiansPerSecond() * duration.getSeconds());
}

void RadiansPerSecond::operator+=(const AngularVelocity& angularVelocity) {
    m_radiansPerSecond += angularVelocity.getRadiansPerSecond();
}

} // namespace sim

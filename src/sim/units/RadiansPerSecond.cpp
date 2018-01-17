#include "RadiansPerSecond.h"

namespace mms {

RadiansPerSecond::RadiansPerSecond() {
    m_radiansPerSecond = 0;
}

RadiansPerSecond::RadiansPerSecond(double radiansPerSecond) {
    m_radiansPerSecond = radiansPerSecond;
}

RadiansPerSecond::RadiansPerSecond(const AngularVelocity& angularVelocity) {
    m_radiansPerSecond = angularVelocity.getRadiansPerSecond();
}

RadiansPerSecond RadiansPerSecond::operator+(const AngularVelocity& angularVelocity) const {
    return RadiansPerSecond(getRadiansPerSecond() + angularVelocity.getRadiansPerSecond());
}

RadiansPerSecond RadiansPerSecond::operator*(double factor) const {
    return RadiansPerSecond(getRadiansPerSecond() * factor);
}

RadiansPerSecond RadiansPerSecond::operator/(double factor) const {
    return RadiansPerSecond(getRadiansPerSecond() / factor);
}

double RadiansPerSecond::operator/(const AngularVelocity& angularVelocity) const {
    return getRadiansPerSecond() / angularVelocity.getRadiansPerSecond();
}

Angle RadiansPerSecond::operator*(const Duration& duration) const {
    return Angle::Radians(getRadiansPerSecond() * duration.getSeconds());
}

MetersPerSecond RadiansPerSecond::operator*(const Distance& radius) const {
    return MetersPerSecond(getRadiansPerSecond() * radius.getMeters());
}

void RadiansPerSecond::operator+=(const AngularVelocity& angularVelocity) {
    m_radiansPerSecond += angularVelocity.getRadiansPerSecond();
}

} // namespace mms

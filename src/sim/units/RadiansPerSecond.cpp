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

} // namespace sim

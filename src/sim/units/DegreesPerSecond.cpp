#include "DegreesPerSecond.h"

namespace sim {

DegreesPerSecond::DegreesPerSecond(double radiansPerSecond) {
    m_radiansPerSecond = radiansPerSecond;
}

DegreesPerSecond::DegreesPerSecond(const AngularVelocity& angularVelocity) {
    m_radiansPerSecond = angularVelocity.getRadiansPerSecond();
}

} // namespace sim

#include "DegreesPerSecond.h"

#include "../CPMath.h"

namespace sim {

DegreesPerSecond::DegreesPerSecond(double degreesPerSecond) {
    static const double radiansPerDegree = M_TWOPI / 360.0;
    m_radiansPerSecond = radiansPerDegree * degreesPerSecond;
}

DegreesPerSecond::DegreesPerSecond(const AngularVelocity& angularVelocity) {
    m_radiansPerSecond = angularVelocity.getRadiansPerSecond();
}

} // namespace sim

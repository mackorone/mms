#include "DegreesPerSecond.h"

#include "../CPMath.h"

namespace sim {

DegreesPerSecond::DegreesPerSecond(double degreesPerSecond) {
    m_radiansPerSecond = degreesPerSecond / 180 * M_PI;
}

DegreesPerSecond::DegreesPerSecond(const AngularVelocity& angularVelocity) {
    m_radiansPerSecond = angularVelocity.getRadiansPerSecond();
}

} // namespace sim

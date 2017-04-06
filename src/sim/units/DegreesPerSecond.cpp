#include "DegreesPerSecond.h"

#include <QtMath>

namespace mms {

DegreesPerSecond::DegreesPerSecond(double degreesPerSecond) {
    static const double radiansPerDegree = 2 * M_PI / 360.0;
    m_radiansPerSecond = radiansPerDegree * degreesPerSecond;
}

DegreesPerSecond::DegreesPerSecond(const AngularVelocity& angularVelocity) {
    m_radiansPerSecond = angularVelocity.getRadiansPerSecond();
}

} // namespace mms

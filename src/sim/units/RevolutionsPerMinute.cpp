#include "RevolutionsPerMinute.h"

#include "../CPMath.h"

namespace sim {

RevolutionsPerMinute::RevolutionsPerMinute(double revolutionsPerMinute) {
    static const double minutesPerSecond = 1.0 / 60.0;
    static const double radiansPerRevolution = M_TWOPI;
    m_radiansPerSecond = radiansPerRevolution * revolutionsPerMinute * minutesPerSecond;
}

RevolutionsPerMinute::RevolutionsPerMinute(const AngularVelocity& angularVelocity) {
    m_radiansPerSecond = angularVelocity.getRadiansPerSecond();
}

} // namespace sim

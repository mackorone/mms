#include "RevolutionsPerMinute.h"

#include <QtMath>

namespace mms {

RevolutionsPerMinute::RevolutionsPerMinute(double revolutionsPerMinute) {
    static const double minutesPerSecond = 1.0 / 60.0;
    static const double radiansPerRevolution = 2 * M_PI;
    m_radiansPerSecond = radiansPerRevolution * revolutionsPerMinute * minutesPerSecond;
}

RevolutionsPerMinute::RevolutionsPerMinute(const AngularVelocity& angularVelocity) {
    m_radiansPerSecond = angularVelocity.getRadiansPerSecond();
}

} // namespace mms

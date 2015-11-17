#include "RevolutionsPerSecond.h"

#include "../CPMath.h"

namespace sim {

RevolutionsPerSecond::RevolutionsPerSecond(double revolutionsPerSecond) {
    m_radiansPerSecond = revolutionsPerSecond * M_TWOPI;
}

RevolutionsPerSecond::RevolutionsPerSecond(const AngularVelocity& angularVelocity) {
    m_radiansPerSecond = angularVelocity.getRadiansPerSecond();
}

} // namespace sim

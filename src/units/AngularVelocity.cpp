#include "AngularVelocity.h"

namespace sim {

AngularVelocity::AngularVelocity() : m_radiansPerSecond(0) {
}

AngularVelocity::~AngularVelocity() {
}

double AngularVelocity::getRadiansPerSecond() const {
    return m_radiansPerSecond;
}

} // namespace sim

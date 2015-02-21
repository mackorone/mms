#include "AngularVelocity.h"

namespace sim {

AngularVelocity::AngularVelocity() : m_radiansPerSecond(0) {
}

AngularVelocity::~AngularVelocity() {
}

float AngularVelocity::getRadiansPerSecond() const {
    return m_radiansPerSecond;
}

} // namespace sim

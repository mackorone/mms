#include "Angle.h"

#include <cmath>

namespace sim {

Angle::Angle() : m_radians(0) {
}

Angle::~Angle() {
}

float Angle::getRadians() const {
    return m_radians;
}

float Angle::getDegrees() const {
    return getRadians() * 360 / (2 * M_PI);
}

} // namespace sim

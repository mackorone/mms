#include <CrossPlatformMath.h>

#include "Angle.h"

namespace sim {

Angle::Angle() : m_radians(0) {
}

Angle::~Angle() {
}

double Angle::getRadians() const {
    return fmod(m_radians + 2*M_PI, 2*M_PI); // Ensure [0, 2pi)
}

double Angle::getDegrees() const {
    return getRadians() * 360 / (2 * M_PI);
}

bool Angle::operator<(const Angle& angle) {
    return getRadians() < angle.getRadians();
}

} // namespace sim

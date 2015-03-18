#include "Radians.h"

namespace sim {

Radians::Radians() {
    m_radians = 0.0;
}

Radians::Radians(float radians) {
    m_radians = radians;
}

Radians::Radians(const Angle& angle) {
    m_radians = angle.getRadians();
}

Radians Radians::operator+(const Angle& angle) const {
    return Radians(getRadians() + angle.getRadians());
}

Radians Radians::operator*(float factor) const {
    return Radians(getRadians() * factor);
}

float Radians::operator/(const Angle& angle) const {
    return getRadians() / angle.getRadians();
}

void Radians::operator+=(const Angle& angle) {
    m_radians += angle.getRadians();
}

} // namespace sim

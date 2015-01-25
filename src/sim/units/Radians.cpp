#include "Radians.h"

namespace sim {

Radians::Radians(float radians) {
    m_radians = radians;
}

Radians::Radians(const Angle& angle) {
    m_radians = angle.getRadians();
}

Radians Radians::operator+(const Angle& angle) const {
    return Radians(getRadians() + angle.getRadians());
}

} // namespace sim

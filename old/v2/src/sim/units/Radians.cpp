#include "Radians.h"

namespace sim {

Radians::Radians() {
    m_radians = 0.0;
}

Radians::Radians(double radians) {
    m_radians = radians;
}

Radians::Radians(const Angle& angle) {
    m_radians = angle.getRadiansNotBounded();
}

Radians Radians::operator+(const Angle& angle) const {
    return Radians(getRadiansNotBounded() + angle.getRadiansNotBounded());
}

Radians Radians::operator-(const Angle& angle) const {
    return Radians(getRadiansNotBounded() - angle.getRadiansNotBounded());
}

Radians Radians::operator*(double factor) const {
    return Radians(getRadiansNotBounded() * factor);
}

Radians Radians::operator/(double factor) const {
    return Radians(getRadiansNotBounded() / factor);
}

void Radians::operator+=(const Angle& angle) {
    m_radians += angle.getRadiansNotBounded();
}

void Radians::operator-=(const Angle& angle) {
    m_radians -= angle.getRadiansNotBounded();
}

} // namespace sim

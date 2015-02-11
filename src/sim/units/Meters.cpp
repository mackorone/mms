#include "Meters.h"

namespace sim {

Meters::Meters(float meters) {
    m_meters = meters;
}

Meters::Meters(const Distance& distance) {
    m_meters = distance.getMeters();
}

Meters Meters::operator+(const Distance& distance) const {
    return Meters(getMeters() + distance.getMeters());
}

Meters Meters::operator-(const Distance& distance) const {
    return Meters(getMeters() - distance.getMeters());
}

Meters Meters::operator*(float factor) const {
    return Meters(getMeters() * factor);
}

Meters Meters::operator/(float factor) const {
    return Meters(getMeters() / factor);
}

} // namespace sim

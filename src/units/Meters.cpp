#include "Meters.h"

namespace sim {

Meters::Meters() {
    m_meters = 0.0;
}

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

float Meters::operator/(const Distance& distance) const {
    return getMeters() / distance.getMeters();
}

MetersSquared Meters::operator*(const Distance& distance) const {
    return MetersSquared(getMeters() * distance.getMeters());
}

void Meters::operator+=(const Distance& distance) {
    m_meters += distance.getMeters();
}

} // namespace sim

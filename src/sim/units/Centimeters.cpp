#include "Centimeters.h"

namespace sim {

Centimeters::Centimeters(double centimeters) {
    m_meters = centimeters / 100;
}

Centimeters::Centimeters(const Distance& distance) {
    m_meters = distance.getMeters();
}

Centimeters Centimeters::operator+(const Distance& distance) const {
    return Centimeters(getCentimeters() + distance.getCentimeters());
}

} // namespace sim

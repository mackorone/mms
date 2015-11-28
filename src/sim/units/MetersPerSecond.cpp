#include "MetersPerSecond.h"

namespace sim {

MetersPerSecond::MetersPerSecond() {
    m_metersPerSecond = 0;
}

MetersPerSecond::MetersPerSecond(double metersPerSecond) {
    m_metersPerSecond = metersPerSecond;
}

MetersPerSecond::MetersPerSecond(const Speed& speed) {
    m_metersPerSecond = speed.getMetersPerSecond();
}

MetersPerSecond MetersPerSecond::operator+(const Speed& speed) const {
    return MetersPerSecond(getMetersPerSecond() + speed.getMetersPerSecond());
}

MetersPerSecond MetersPerSecond::operator-(const Speed& speed) const {
    return MetersPerSecond(getMetersPerSecond() - speed.getMetersPerSecond());
}

MetersPerSecond MetersPerSecond::operator*(double factor) const {
    return MetersPerSecond(getMetersPerSecond() * factor);
}

MetersPerSecond MetersPerSecond::operator/(double factor) const {
    return MetersPerSecond(getMetersPerSecond() / factor);
}

double MetersPerSecond::operator/(const Speed& speed) const {
    return getMetersPerSecond() / speed.getMetersPerSecond();
}

Meters MetersPerSecond::operator*(const Duration& duration) const {
    return Meters(getMetersPerSecond() * duration.getSeconds());
}

void MetersPerSecond::operator+=(const Speed& speed) {
    m_metersPerSecond += speed.getMetersPerSecond();
}

} // namespace sim

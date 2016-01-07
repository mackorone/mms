#include "Distance.h"

namespace sim {

Distance::Distance() : m_meters(0.0) {
}

Distance::~Distance() {
}

double Distance::getMeters() const {
    return m_meters;
}

double Distance::getCentimeters() const {
    static const double centimetersPerMeter = 100.0;
    return centimetersPerMeter * getMeters();
}

bool Distance::operator==(const Distance& distance) const {
    return getMeters() == distance.getMeters();
}

bool Distance::operator!=(const Distance& distance) const {
    return (!(operator==(distance)));
}

bool Distance::operator<(const Distance& distance) const {
    return getMeters() < distance.getMeters();
}

Seconds Distance::operator/(const Speed& speed) const {
    return Seconds(getMeters() / speed.getMetersPerSecond());
}

} // namespace sim

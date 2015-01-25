#include "MetersPerSecond.h"

namespace sim {

MetersPerSecond::MetersPerSecond(float metersPerSecond) {
    m_metersPerSecond = metersPerSecond;
}

MetersPerSecond::MetersPerSecond(const Speed& speed) {
    m_metersPerSecond = speed.getMetersPerSecond();
}

MetersPerSecond MetersPerSecond::operator+(const Speed& speed) const {
    return MetersPerSecond(getMetersPerSecond() + speed.getMetersPerSecond());
}

} // namespace sim

#include "Speed.h"

namespace sim {

Speed::Speed() : m_metersPerSecond(0) {
}

Speed::~Speed() {
}

double Speed::getMetersPerSecond() const {
    return m_metersPerSecond;
}

bool Speed::operator<(const Speed& speed) const {
    return m_metersPerSecond < speed.getMetersPerSecond();
}

} // namespace sim

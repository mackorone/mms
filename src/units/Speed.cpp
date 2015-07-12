#include "Speed.h"

namespace sim {

Speed::Speed() : m_metersPerSecond(0) {
}

Speed::~Speed() {
}

double Speed::getMetersPerSecond() const {
    return m_metersPerSecond;
}

} // namespace sim

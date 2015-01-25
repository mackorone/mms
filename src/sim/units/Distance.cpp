#include "Distance.h"

namespace sim {

Distance::Distance() : m_meters(0) {
}

Distance::~Distance() {
}

float Distance::getMeters() const {
    return m_meters;
}

float Distance::getCentimeters() const {
    return getMeters() * 100;
}

} // namespace sim

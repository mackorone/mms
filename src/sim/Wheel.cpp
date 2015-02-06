#include "Wheel.h"

namespace sim {

Wheel::Wheel(const Distance& radius, const Distance& width, const Coordinate& position) :
        m_radius(radius), m_width(width), m_position(position), m_angularVelocity(0.0) {
}

Meters Wheel::getRadius() {
    return m_radius;
}

Meters Wheel::getWidth() {
    return m_width;
}

Cartesian Wheel::getPosition() {
    return m_position;
}

RadiansPerSecond Wheel::getAngularVelocity() {
    return m_angularVelocity;
}

} // namespace sim

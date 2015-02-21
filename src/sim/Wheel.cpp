#include "Wheel.h"

namespace sim {

Wheel::Wheel(const Distance& radius, const Distance& width, const Coordinate& position) :
        m_radius(radius), m_width(width), m_position(position), m_rotation(0),
        m_angularVelocity(0.0) {
}

Wheel::Wheel() : m_radius(0), m_width(0), m_position(Meters(0.0), Meters(0.0)), m_rotation(0),
        m_angularVelocity(0.0) {
}


Meters Wheel::getRadius() const {
    return m_radius;
}

Meters Wheel::getWidth() const {
    return m_width;
}

Cartesian Wheel::getPosition() const {
    return m_position;
}

RadiansPerSecond Wheel::getAngularVelocity() const {
    return m_angularVelocity;
}

void Wheel::setAngularVelocity(const AngularVelocity& angularVelocity) {
    m_angularVelocity = RadiansPerSecond(angularVelocity);
}

void Wheel::rotate(const Angle& angle) {
    m_rotation += angle;
}

Radians Wheel::popRotation() {
    Radians rotation = m_rotation;
    m_rotation = Radians(0);
    return rotation;
}

} // namespace sim

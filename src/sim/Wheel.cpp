#include "Wheel.h"

#include "CPMath.h"

namespace sim {

Wheel::Wheel(
        const Distance& diameter,
        const Distance& width,
        const Coordinate& position,
        const Angle& direction) :
        m_diameter(diameter),
        m_initialPosition(position),
        m_initialDirection(direction),
        m_angularVelocity(RadiansPerSecond(0.0)) {

    // Initialize the polygon for the wheel
    Meters halfWidth = Meters(width) / 2.0;
    Meters radius = Meters(diameter) / 2.0;
    std::vector<Cartesian> polygon;
    polygon.push_back(Cartesian(halfWidth * -1, Meters(radius) * -1) + position);
    polygon.push_back(Cartesian(halfWidth * -1, Meters(radius) *  1) + position);
    polygon.push_back(Cartesian(halfWidth *  1, Meters(radius) *  1) + position);
    polygon.push_back(Cartesian(halfWidth *  1, Meters(radius) * -1) + position);
    m_initialPolygon = Polygon(polygon);
}

Meters Wheel::getDiameter() const {
    return m_diameter;
}

Cartesian Wheel::getInitialPosition() const {
    return m_initialPosition;
}

Radians Wheel::getInitialDirection() const {
    return m_initialDirection;
}

Polygon Wheel::getInitialPolygon() const {
    return m_initialPolygon;
}

RadiansPerSecond Wheel::getAngularVelocity() const {
    return m_angularVelocity;
}

void Wheel::setAngularVelocity(const AngularVelocity& angularVelocity) {
    m_angularVelocity = angularVelocity;
}

} // namespace sim

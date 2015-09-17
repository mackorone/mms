#include "Wheel.h"

#include "CPMath.h"

namespace sim {

Wheel::Wheel() : m_radius(Meters(0.0)), m_initialTranslation(Cartesian(Meters(0.0), Meters(0.0))),
        m_initialRotation(Radians(M_PI / 2.0)), m_angularVelocity(RadiansPerSecond(0.0)) {
}

Wheel::Wheel(const Distance& radius, const Distance& width, const Coordinate& position) :
        m_radius(radius), m_initialTranslation(position), m_initialRotation(Radians(M_PI / 2.0)),
        m_angularVelocity(RadiansPerSecond(0.0)) {

    // Initialize the polygon for the wheel
    Meters halfWidth = Meters(width) / 2.0;
    std::vector<Cartesian> polygon;
    polygon.push_back(Cartesian(halfWidth * -1, Meters(radius) * -1) + position);
    polygon.push_back(Cartesian(halfWidth *  1, Meters(radius) * -1) + position);
    polygon.push_back(Cartesian(halfWidth *  1, Meters(radius) *  1) + position);
    polygon.push_back(Cartesian(halfWidth * -1, Meters(radius) *  1) + position);
    m_initialPolygon = Polygon(polygon);
}

Meters Wheel::getRadius() const {
    return m_radius;
}

Cartesian Wheel::getInitialTranslation() const {
    return m_initialTranslation;
}

Radians Wheel::getInitialRotation() const {
    return m_initialRotation;
}

Polygon Wheel::getInitialPolygon() const {
    return m_initialPolygon;
}

RadiansPerSecond Wheel::getAngularVelocity() const {
    return m_angularVelocity;
}

void Wheel::setAngularVelocity(const AngularVelocity& angularVelocity) {
    m_angularVelocity = RadiansPerSecond(angularVelocity);
}

} // namespace sim

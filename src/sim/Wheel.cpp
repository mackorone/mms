#include "Wheel.h"

namespace sim {

Wheel::Wheel() : m_radius(Meters(0.0)), m_width(Meters(0.0)), m_position(Cartesian(Meters(0.0), Meters(0.0))),
        m_angularVelocity(0.0) {
}

Wheel::Wheel(const Distance& radius, const Distance& width, const Coordinate& position) :
        m_radius(radius), m_width(width), m_position(position), m_angularVelocity(0.0) {

    // Initialize the polygon for the wheel
    Meters halfWidth = Meters(width) / 2.0;
    std::vector<Cartesian> polygon;
    polygon.push_back(Cartesian(halfWidth * -1, Meters(radius) * -1) + position);
    polygon.push_back(Cartesian(halfWidth *  1, Meters(radius) * -1) + position);
    polygon.push_back(Cartesian(halfWidth *  1, Meters(radius) *  1) + position);
    polygon.push_back(Cartesian(halfWidth * -1, Meters(radius) *  1) + position);
    m_polygon = Polygon(polygon);
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

Polygon Wheel::getPolygon() const {
    return m_polygon;
}

RadiansPerSecond Wheel::getAngularVelocity() const {
    return m_angularVelocity;
}

void Wheel::setAngularVelocity(const AngularVelocity& angularVelocity) {
    m_angularVelocity = RadiansPerSecond(angularVelocity);
}

} // namespace sim

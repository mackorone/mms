#include "Wheel.h"

#include "CPMath.h"

namespace sim {

Wheel::Wheel(
        const Distance& diameter,
        const Distance& width,
        const Coordinate& position,
        const Angle& direction) :
        m_diameter(diameter),
        m_halfWidth(Meters(width) / 2.0),
        m_initialPosition(position),
        m_initialDirection(direction),
        m_angularVelocity(RadiansPerSecond(0.0)) {

    Meters radius = Meters(diameter) / 2.0;
    std::vector<Cartesian> polygon;
    polygon.push_back(Cartesian(Meters(radius) * -1, m_halfWidth * -1) + m_initialPosition);
    polygon.push_back(Cartesian(Meters(radius) *  1, m_halfWidth * -1) + m_initialPosition);
    polygon.push_back(Cartesian(Meters(radius) *  1, m_halfWidth *  1) + m_initialPosition);
    polygon.push_back(Cartesian(Meters(radius) * -1, m_halfWidth *  1) + m_initialPosition);
    m_initialPolygon = Polygon(polygon).rotateAroundPoint(m_initialDirection, m_initialPosition);

    m_speedIndicatorPolygon = Polygon({
        Cartesian(m_initialPosition),
        Cartesian(m_initialPosition) + Cartesian(Meters(0), Meters(m_halfWidth *  1)),
        Cartesian(m_initialPosition) + Cartesian(Meters(0), Meters(m_halfWidth * -1)),
    }).rotateAroundPoint(m_initialDirection, m_initialPosition);
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

Polygon Wheel::getSpeedIndicatorPolygon() const {
    return m_speedIndicatorPolygon;
}

RadiansPerSecond Wheel::getAngularVelocity() const {
    return m_angularVelocity;
}

void Wheel::setAngularVelocity(const AngularVelocity& angularVelocity) {
    m_angularVelocity = angularVelocity;
    updateSpeedIndicatorPolygon(angularVelocity);
}

void Wheel::updateSpeedIndicatorPolygon(const AngularVelocity& angularVelocity) {
    Meters radius = m_diameter / 2;
    double fractionOfMaxSpeed = angularVelocity / RadiansPerSecond(20); // TODO: MACK - max speed and max accel
    m_speedIndicatorPolygon = Polygon({
        Cartesian(m_initialPosition) + Cartesian(radius * fractionOfMaxSpeed, Meters(0)),
        Cartesian(m_initialPosition) + Cartesian(Meters(0), Meters(m_halfWidth *  1)),
        Cartesian(m_initialPosition) + Cartesian(Meters(0), Meters(m_halfWidth * -1)),
    }).rotateAroundPoint(m_initialDirection, m_initialPosition);
}

} // namespace sim

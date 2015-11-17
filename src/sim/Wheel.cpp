#include "Wheel.h"

#include "CPMath.h"
#include "units/RevolutionsPerSecond.h"

namespace sim {

Wheel::Wheel(
        const Distance& diameter,
        const Distance& width,
        const Coordinate& position,
        const Angle& direction) :
        m_radius(Meters(diameter) / 2.0),
        m_halfWidth(Meters(width) / 2.0),
        m_initialPosition(position),
        m_initialDirection(direction),
        m_angularVelocity(RadiansPerSecond(0.0)) {

    // Create the initial wheel polygon
    std::vector<Cartesian> polygon;
    polygon.push_back(Cartesian(Meters(m_radius) * -1, m_halfWidth * -1) + m_initialPosition);
    polygon.push_back(Cartesian(Meters(m_radius) *  1, m_halfWidth * -1) + m_initialPosition);
    polygon.push_back(Cartesian(Meters(m_radius) *  1, m_halfWidth *  1) + m_initialPosition);
    polygon.push_back(Cartesian(Meters(m_radius) * -1, m_halfWidth *  1) + m_initialPosition);
    m_initialPolygon = Polygon(polygon).rotateAroundPoint(m_initialDirection, m_initialPosition);

    // Create the initial speed indicator polygon
    m_speedIndicatorPolygon = getSpeedIndicatorPolygon(m_angularVelocity);
}

Meters Wheel::getRadius() const {
    return m_radius;
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
    m_speedIndicatorPolygon = getSpeedIndicatorPolygon(angularVelocity);
}

Polygon Wheel::getSpeedIndicatorPolygon(const AngularVelocity& angularVelocity) {
    double fractionOfMaxSpeed = angularVelocity / RevolutionsPerSecond(1.0); // TODO: MACK - max speed and max accel
    return Polygon({
        Cartesian(m_initialPosition) + Cartesian(m_radius * fractionOfMaxSpeed, Meters(0)),
        Cartesian(m_initialPosition) + Cartesian(Meters(0), Meters(m_halfWidth *  1)),
        Cartesian(m_initialPosition) + Cartesian(Meters(0), Meters(m_halfWidth * -1)),
    }).rotateAroundPoint(m_initialDirection, m_initialPosition);
}

} // namespace sim

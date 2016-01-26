#include "Wheel.h"

#include "CPMath.h"
#include "GeometryUtilities.h"

namespace sim {

Wheel::Wheel(
        const Distance& diameter,
        const Distance& width,
        const Coordinate& position,
        const Angle& direction,
        const AngularVelocity& maxAngularVelocityMagnitude,
        EncoderType encoderType,
        double encoderTicksPerRevolution) :
        m_radius(Meters(diameter) / 2.0),
        m_halfWidth(Meters(width) / 2.0),
        m_initialPosition(position),
        m_initialDirection(direction),
        m_speedIndicatorPolygonNeedsToBeUpdated(true),
        m_angularVelocity(RadiansPerSecond(0.0)),
        m_maxAngularVelocityMagnitude(maxAngularVelocityMagnitude),
        m_encoderType(encoderType),
        m_encoderTicksPerRevolution(encoderTicksPerRevolution),
        m_absoluteRotation(Radians(0)),
        m_relativeRotation(Radians(0)) {

    // Create the initial wheel polygon
    std::vector<Cartesian> polygon;
    polygon.push_back(Cartesian(Meters(m_radius) * -1, m_halfWidth * -1));
    polygon.push_back(Cartesian(Meters(m_radius) *  1, m_halfWidth * -1));
    polygon.push_back(Cartesian(Meters(m_radius) *  1, m_halfWidth *  1));
    polygon.push_back(Cartesian(Meters(m_radius) * -1, m_halfWidth *  1));
    m_initialPolygon =
        Polygon(polygon)
            .translate(m_initialPosition)
            .rotateAroundPoint(m_initialDirection, m_initialPosition);
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

const Polygon& Wheel::getInitialPolygon() const {
    return m_initialPolygon;
}

const Polygon& Wheel::getSpeedIndicatorPolygon() const {
    if (m_speedIndicatorPolygonNeedsToBeUpdated) {
        m_speedIndicatorPolygon = getSpeedIndicatorPolygon(m_angularVelocity);
        m_speedIndicatorPolygonNeedsToBeUpdated = false;
    }
    return m_speedIndicatorPolygon;
}

RadiansPerSecond Wheel::getAngularVelocity() const {
    return m_angularVelocity;
}

RadiansPerSecond Wheel::getMaxAngularVelocityMagnitude() const {
    return m_maxAngularVelocityMagnitude;
}

void Wheel::setAngularVelocity(const AngularVelocity& angularVelocity) {
    m_angularVelocity = angularVelocity;
    m_speedIndicatorPolygonNeedsToBeUpdated = true;
}

EncoderType Wheel::getEncoderType() const {
    return m_encoderType;
}

double Wheel::getEncoderTicksPerRevolution() const {
    return m_encoderTicksPerRevolution;
}

int Wheel::readAbsoluteEncoder() const {
    return static_cast<int>(std::floor(
        m_encoderTicksPerRevolution * m_absoluteRotation.getRadiansZeroTo2pi() / M_TWOPI));
}

int Wheel::readRelativeEncoder() const {
    // We use std::trunc instead of std::floor so we round towards zero in the
    // case of a negative relative rotation
    return static_cast<int>(std::trunc(
        m_encoderTicksPerRevolution * m_relativeRotation.getRadiansNotBounded() / M_TWOPI));
}

void Wheel::resetRelativeEncoder() {
    m_relativeRotation = Radians(0);
}

void Wheel::updateRotation(const Angle& angle) {
    m_absoluteRotation += angle;
    m_relativeRotation += angle;
}

Polygon Wheel::getSpeedIndicatorPolygon(const AngularVelocity& angularVelocity) const {
    double fractionOfMaxSpeed = 0.0;
    if (0.0 < getMaxAngularVelocityMagnitude().getRadiansPerSecond()) {
        fractionOfMaxSpeed = angularVelocity / getMaxAngularVelocityMagnitude();
    }
    return Polygon({
        Cartesian(m_initialPosition) + Cartesian(m_radius * fractionOfMaxSpeed, Meters(0)),
        Cartesian(m_initialPosition) + Cartesian(Meters(0), Meters(m_halfWidth *  1)),
        Cartesian(m_initialPosition) + Cartesian(Meters(0), Meters(m_halfWidth * -1)),
    }).rotateAroundPoint(m_initialDirection, m_initialPosition);
}

} // namespace sim

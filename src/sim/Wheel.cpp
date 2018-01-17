#include "Wheel.h"

#include <QVector>
#include <QtMath>

#include "GeometryUtilities.h"

namespace mms {

Wheel::Wheel() :
    m_radius(Meters(0)),
    m_halfWidth(Meters(0)),
    m_initialPosition(Cartesian(Meters(0), Meters(0))),
    m_initialDirection(Angle::Radians(0)),
    m_angularVelocity(RadiansPerSecond(0.0)),
    m_maxAngularVelocityMagnitude(RadiansPerSecond(0)),
    m_encoderType(EncoderType::ABSOLUTE),
    m_encoderTicksPerRevolution(0),
    m_absoluteRotation(Angle::Radians(0)),
    m_relativeRotation(Angle::Radians(0)) {
}

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
        m_angularVelocity(RadiansPerSecond(0.0)),
        m_maxAngularVelocityMagnitude(maxAngularVelocityMagnitude),
        m_encoderType(encoderType),
        m_encoderTicksPerRevolution(encoderTicksPerRevolution),
        m_absoluteRotation(Angle::Radians(0)),
        m_relativeRotation(Angle::Radians(0)) {

    // Create the initial wheel polygon
    QVector<Cartesian> polygon;
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

Angle Wheel::getInitialDirection() const {
    return m_initialDirection;
}

const Polygon& Wheel::getInitialPolygon() const {
    return m_initialPolygon;
}

RadiansPerSecond Wheel::getAngularVelocity() const {
    return m_angularVelocity;
}

RadiansPerSecond Wheel::getMaxAngularVelocityMagnitude() const {
    return m_maxAngularVelocityMagnitude;
}

void Wheel::setAngularVelocity(const AngularVelocity& angularVelocity) {
    m_angularVelocity = angularVelocity;
}

EncoderType Wheel::getEncoderType() const {
    return m_encoderType;
}

double Wheel::getEncoderTicksPerRevolution() const {
    return m_encoderTicksPerRevolution;
}

int Wheel::readAbsoluteEncoder() const {
    return static_cast<int>(std::floor(
        m_encoderTicksPerRevolution *
        m_absoluteRotation.getRadiansZeroTo2pi() /
        (2 * M_PI)));
}

int Wheel::readRelativeEncoder() const {
    // We use std::trunc instead of std::floor to ensure
    // we round negative relative rotations towards zero
    return static_cast<int>(std::trunc(
        m_encoderTicksPerRevolution * 
        m_relativeRotation.getRadiansUnbounded() /
        (2 * M_PI)));
}

void Wheel::resetRelativeEncoder() {
    m_relativeRotation = Angle::Radians(0);
}

void Wheel::updateRotation(const Angle& angle) {
    m_absoluteRotation += angle;
    m_relativeRotation += angle;
}

} // namespace mms

#include "Wheel.h"

#include <QVector>
#include <QtMath>

namespace mms {

Wheel::Wheel() :
    m_unitForwardEffect(Speed()),
    m_unitSidewaysEffect(Speed()),
    m_unitTurnEffect(AngularVelocity()),
    m_maximumSpeed(AngularVelocity()),
    m_currentSpeed(AngularVelocity()),
    m_encoderTicksPerRevolution(0),
    m_absoluteRotation(Angle()),
    m_relativeRotation(Angle()) {
}

Wheel::Wheel(
    const Coordinate& mousePosition,
    const Coordinate& wheelPosition,
    const Angle& mouseDirection,
    const Angle& wheelDirection,
    const Distance& diameter,
    const Distance& width,
    const AngularVelocity& maximumSpeed,
    EncoderType encoderType,
    double encoderTicksPerRevolution
) :
    m_maximumSpeed(maximumSpeed),
    m_currentSpeed(AngularVelocity::RadiansPerSecond(0.0)),
    m_encoderType(encoderType),
    m_encoderTicksPerRevolution(encoderTicksPerRevolution),
    m_absoluteRotation(Angle::Radians(0)),
    m_relativeRotation(Angle::Radians(0)
) {
    // Create the initial wheel polygon
    QVector<Coordinate> polygon;
    Distance radius = diameter / 2.0;
    Distance halfWidth = width / 2.0;
    polygon.push_back(Coordinate::Cartesian(radius * -1, halfWidth * -1));
    polygon.push_back(Coordinate::Cartesian(radius *  1, halfWidth * -1));
    polygon.push_back(Coordinate::Cartesian(radius *  1, halfWidth *  1));
    polygon.push_back(Coordinate::Cartesian(radius * -1, halfWidth *  1));
    m_initialPolygon = Polygon(polygon)
        .translate(wheelPosition)
        .rotateAroundPoint(wheelDirection, wheelPosition);

    // Calculate the effects of this wheel on the mouse
    AngularVelocity oneRadianPerSecond = AngularVelocity::RadiansPerSecond(1.0);
    double forwardComponent = (mouseDirection - wheelDirection).getCos();
    double sideWaysComponent = (mouseDirection - wheelDirection).getSin();
    m_unitForwardEffect = oneRadianPerSecond * radius * forwardComponent;
    m_unitSidewaysEffect = oneRadianPerSecond * radius * sideWaysComponent;
    Coordinate wheelToCenter = mousePosition - wheelPosition;
    double perpendicularComponent =
        (wheelToCenter.getTheta() - wheelDirection).getSin();
    m_unitTurnEffect = AngularVelocity::RadiansPerSecond(
        radius.getMeters() *
        perpendicularComponent *
        (1.0 / wheelToCenter.getRho().getMeters())
    );
}

const Polygon& Wheel::getInitialPolygon() const {
    return m_initialPolygon;
}

WheelEffect Wheel::getMaximumEffect() const {
    return getEffect(getMaximumSpeed());
}

WheelEffect Wheel::update(const Duration& elapsed) {
    Angle angle = m_currentSpeed * elapsed;
    m_absoluteRotation += angle;
    m_relativeRotation += angle;
    return getEffect(m_currentSpeed);
}

const AngularVelocity& Wheel::getMaximumSpeed() const {
    return m_maximumSpeed;
}

const AngularVelocity& Wheel::getCurrentSpeed() const {
    return m_currentSpeed;
}

void Wheel::setSpeed(const AngularVelocity& speed) {
    ASSERT_LE(
        std::abs(speed.getRevolutionsPerMinute()),
        m_maximumSpeed.getRevolutionsPerMinute());
    m_currentSpeed = speed;
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

WheelEffect Wheel::getEffect(const AngularVelocity& speed) const {
    return {
        m_unitForwardEffect * speed.getRadiansPerSecond(),
        m_unitSidewaysEffect * speed.getRadiansPerSecond(),
        m_unitTurnEffect * speed.getRadiansPerSecond()
    };
}

} // namespace mms

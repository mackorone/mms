#pragma once

#include "EncoderType.h"
#include "Polygon.h"
#include "units/AngularVelocity.h"
#include "units/Coordinate.h"
#include "units/Meters.h"

namespace mms {

class Wheel {

public:
    Wheel();
    Wheel(
        const Distance& diameter,
        const Distance& width,
        const Coordinate& position,
        const Angle& direction,
        const AngularVelocity& maxAngularVelocityMagnitude,
        EncoderType encoderType,
        double encoderTicksPerRevolution);

    // Wheel
    Meters getRadius() const;
    Coordinate getInitialPosition() const;
    Angle getInitialDirection() const;
    const Polygon& getInitialPolygon() const;

    // Motor
    AngularVelocity getAngularVelocity() const;
    AngularVelocity getMaxAngularVelocityMagnitude() const;
    void setAngularVelocity(const AngularVelocity& angularVelocity);

    // Encoder
    EncoderType getEncoderType() const;
    double getEncoderTicksPerRevolution() const;
    int readAbsoluteEncoder() const;
    int readRelativeEncoder() const;
    void resetRelativeEncoder();
    void updateRotation(const Angle& angle);

private:

    // Wheel
    Meters m_radius;
    Meters m_halfWidth;
    Coordinate m_initialPosition;
    Angle m_initialDirection;
    Polygon m_initialPolygon;

    // Motor
    AngularVelocity m_angularVelocity;
    AngularVelocity m_maxAngularVelocityMagnitude;

    // Encoder
    EncoderType m_encoderType;
    double m_encoderTicksPerRevolution;
    Angle m_absoluteRotation;
    Angle m_relativeRotation;
};

} // namespace mms

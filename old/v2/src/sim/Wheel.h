#pragma once

#include "EncoderType.h"
#include "Polygon.h"
#include "units/Cartesian.h"
#include "units/Meters.h"
#include "units/RadiansPerSecond.h"

namespace sim {

class Wheel {

public:
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
    Cartesian getInitialPosition() const;
    Radians getInitialDirection() const;
    const Polygon& getInitialPolygon() const;
    const Polygon& getSpeedIndicatorPolygon() const;

    // Motor
    RadiansPerSecond getAngularVelocity() const;
    RadiansPerSecond getMaxAngularVelocityMagnitude() const;
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
    Cartesian m_initialPosition;
    Radians m_initialDirection;
    Polygon m_initialPolygon;
    mutable Polygon m_speedIndicatorPolygon;
    mutable bool m_speedIndicatorPolygonNeedsToBeUpdated;

    // Motor
    RadiansPerSecond m_angularVelocity;
    RadiansPerSecond m_maxAngularVelocityMagnitude;

    // Encoder
    EncoderType m_encoderType;
    double m_encoderTicksPerRevolution;
    Radians m_absoluteRotation;
    Radians m_relativeRotation;

    // Helper function for updating the speed indicator polygon
    Polygon getSpeedIndicatorPolygon(const AngularVelocity& angularVelocity) const;
};

} // namespace sim

#pragma once

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
        double encoderTicksPerRevolution);

    // Wheel
    Meters getRadius() const;
    Cartesian getInitialPosition() const;
    Radians getInitialDirection() const;
    Polygon getInitialPolygon() const;
    Polygon getSpeedIndicatorPolygon() const;

    // Motor
    RadiansPerSecond getAngularVelocity() const;
    RadiansPerSecond getMaxAngularVelocityMagnitude() const;
    void setAngularVelocity(const AngularVelocity& angularVelocity);

    // Encoder
    double getEncoderTicksPerRevolution() const;
    void updateRotation(const Angle& angle);
    int readEncoder() const;

private:

    // Wheel
    Meters m_radius;
    Meters m_halfWidth;
    Cartesian m_initialPosition;
    Radians m_initialDirection;
    Polygon m_initialPolygon;
    Polygon m_speedIndicatorPolygon;

    // Motor
    RadiansPerSecond m_angularVelocity;
    RadiansPerSecond m_maxAngularVelocityMagnitude;

    // Encoder
    double m_encoderTicksPerRevolution;
    Radians m_rotation;

    // Helper function for updating the speed indicator polygon
    Polygon getSpeedIndicatorPolygon(const AngularVelocity& angularVelocity);
};

} // namespace sim

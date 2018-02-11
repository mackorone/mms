#pragma once

#include "EncoderType.h"
#include "Polygon.h"
#include "WheelEffect.h"

#include "units/AngularVelocity.h"
#include "units/Coordinate.h"
#include "units/Distance.h"
#include "units/Speed.h"

namespace mms {

class Wheel {

public:

    Wheel();
    Wheel(
        const Coordinate& mousePosition,
        const Coordinate& wheelPosition,
        const Angle& mouseDirection,
        const Angle& wheelDirection,
        const Distance& diameter,
        const Distance& width,
        const AngularVelocity& maximumSpeed,
        EncoderType encoderType,
        double encoderTicksPerRevolution);

    // Wheel
    const Polygon& getInitialPolygon() const;
    WheelEffect getMaximumEffect() const;
    WheelEffect update(const Duration& elapsed);

    // Motor
    const AngularVelocity& getMaximumSpeed() const;
    const AngularVelocity& getCurrentSpeed() const;
    void setSpeed(const AngularVelocity& speed);

    // Encoder
    EncoderType getEncoderType() const;
    double getEncoderTicksPerRevolution() const;
    int readAbsoluteEncoder() const;
    int readRelativeEncoder() const;
    void resetRelativeEncoder();

private:

    // Wheel
    Polygon m_initialPolygon;

    // The effects this wheel (on the mouse) if the
    // angular velocity is 1.0 rad/s (hence "unit")
    Speed m_unitForwardEffect;
    Speed m_unitSidewaysEffect;
    AngularVelocity m_unitTurnEffect;

    // Motor
    AngularVelocity m_maximumSpeed;
    AngularVelocity m_currentSpeed;

    // Encoder
    EncoderType m_encoderType;
    double m_encoderTicksPerRevolution;
    Angle m_absoluteRotation;
    Angle m_relativeRotation;

    // Helper function
    WheelEffect getEffect(const AngularVelocity& speed) const;

};

} // namespace mms

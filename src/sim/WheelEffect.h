#pragma once

#include "units/AngularVelocity.h"
#include "units/Coordinate.h"
#include "units/Speed.h"
#include "Wheel.h"

namespace mms {

class WheelEffect {

public:
    WheelEffect();
    WheelEffect(
        const Coordinate& initialTranslation,
        const Angle& initialRotation,
        const Wheel& wheel);

    // The rates at which this wheel would cause the mouse to move forward,
    // move sideways, and turn, respectively, when it has a certain speed.
    std::tuple<Speed, Speed, AngularVelocity> getEffects(
        const AngularVelocity& wheelSpeed) const;

private:
    // The rates if the wheel had a speed of 1.0 rad/s (hence "unit")
    Speed m_unitForwardEffect;
    Speed m_unitSidewaysEffect;
    AngularVelocity m_unitTurnEffect;

};

} // namespace mms

#pragma once

#include "units/AngularVelocity.h"
#include "units/Cartesian.h"
#include "units/MetersPerSecond.h"
#include "Wheel.h"

namespace mms {

class WheelEffect {

public:
    WheelEffect();
    WheelEffect(
        const Cartesian& initialTranslation,
        const Angle& initialRotation,
        const Wheel& wheel);

    // The rates at which this wheel would cause the mouse to move forward,
    // move sideways, and turn, respectively, when it has a certain speed.
    std::tuple<MetersPerSecond, MetersPerSecond, AngularVelocity> getEffects(
        const AngularVelocity& wheelSpeed) const;

private:
    // The rates if the wheel had a speed of 1.0 rad/s (hence "unit")
    MetersPerSecond m_unitForwardEffect;
    MetersPerSecond m_unitSidewaysEffect;
    AngularVelocity m_unitTurnEffect;

};

} // namespace mms

#pragma once

#include "units/Cartesian.h"
#include "units/RadiansPerSecond.h"
#include "Wheel.h"

namespace sim {

class WheelEffect {

public:
    WheelEffect(
        const Cartesian& initialTranslation,
        const Radians& initialRotation,
        const Wheel& wheel);

    // The rates at which this wheel would cause the mouse to move forward,
    // move sideways, and turn, respectively, when it has a certain speed.
    std::tuple<MetersPerSecond, MetersPerSecond, RadiansPerSecond> getEffects(
        const RadiansPerSecond& wheelSpeed) const;

private:
    // The rates if the wheel had a speed of 1.0 rad/s (hence "unit")
    MetersPerSecond m_unitForwardEffect;
    MetersPerSecond m_unitSidewaysEffect;
    RadiansPerSecond m_unitTurnEffect;

};

} // namespace sim

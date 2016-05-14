#include "WheelEffect.h"

namespace sim {

WheelEffect::WheelEffect(
        const Cartesian& initialTranslation,
        const Radians& initialRotation,
        const Wheel& wheel) {

    m_unitForwardEffect = MetersPerSecond(
        wheel.getRadius().getMeters() * 
        (initialRotation - wheel.getInitialDirection()).getCos()
    );

    m_unitSidewaysEffect = MetersPerSecond(
        wheel.getRadius().getMeters() * 
        (initialRotation - wheel.getInitialDirection()).getSin()
    );

    Cartesian wheelToCenter = initialTranslation - wheel.getInitialPosition();
    m_unitTurnEffect = RadiansPerSecond(
        wheel.getRadius().getMeters() * 
        (wheelToCenter.getTheta() - wheel.getInitialDirection()).getSin() *
        (1.0 / wheelToCenter.getRho().getMeters())
    );
}

std::tuple<MetersPerSecond, MetersPerSecond, RadiansPerSecond> WheelEffect::getEffects(
        const RadiansPerSecond& wheelSpeed) const {
    return std::make_tuple(
        m_unitForwardEffect * wheelSpeed.getRadiansPerSecond(),
        m_unitSidewaysEffect * wheelSpeed.getRadiansPerSecond(),
        m_unitTurnEffect * wheelSpeed.getRadiansPerSecond()
    );
}

} // namespace sim

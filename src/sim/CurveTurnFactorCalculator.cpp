#include "CurveTurnFactorCalculator.h"

#include <QMapIterator>

namespace sim {

CurveTurnFactorCalculator::CurveTurnFactorCalculator() :
    m_unitForwardComponent(0.0),
    m_unitTurnComponent(0.0) {
}

CurveTurnFactorCalculator::CurveTurnFactorCalculator(
        const QMap<QString, Wheel>& wheels,
        const QMap<QString, WheelEffect>& wheelEffects,
        const QMap<QString, std::pair<double, double>>& wheelSpeedAdjustmentFactors) {

    // TODO: upforgrabs
    // Currently, this logic assumes that when the mouse has its wheel
    // speeds set for moving forward, that it does not turn at all, and vice
    // versa. This is not the case for mice where the wheels don't have
    // rotational symmetry. What we really care about is that the mouse moves
    // in the forward direction (relative to the initial direction of the
    // mouse) as much as intended. The sideways movement (relative to to the
    // initial direction of the mouse) can vary as needed.

    // Determine the total forward and turn rate of change from all wheels
    MetersPerSecond totalForwardRateOfChange(0);
    RadiansPerSecond totalRadialRateOfChange(0);

    QMapIterator<QString, Wheel> iterator(wheels);    
    while (iterator.hasNext()) {
        auto pair = iterator.next();
        auto wheelName = pair.key();
        auto wheel = pair.value();

        // For each of the wheel speed adjustment factors, calculate the wheel's
        // contributions. Remember that each of these factors corresponds to
        // the fraction of the max wheel speed such that the mouse performs a
        // particular movement (moving forward or turning) most optimally.
        SIM_ASSERT_TR(wheelSpeedAdjustmentFactors.contains(wheelName));
        std::pair<double, double> adjustmentFactors =
            wheelSpeedAdjustmentFactors.value(wheelName);

        for (double adjustmentFactor : {adjustmentFactors.first, adjustmentFactors.second}) {

            std::tuple<MetersPerSecond, MetersPerSecond, RadiansPerSecond> effects =
                wheelEffects.value(wheelName).getEffects(
                    wheel.getMaxAngularVelocityMagnitude() * adjustmentFactor);

            totalForwardRateOfChange += std::get<0>(effects);
            totalRadialRateOfChange += std::get<2>(effects);
        }
    }

    // The main idea here is that, for a curve turn, we want the mouse to move
    // forward a distance equal to the length of the arc we'd like it to travel.
    // So we'd like to find a pair of factors, A and B, such that:
    //
    //  totalForwardRateOfChange * A   totalDistance
    //  ---------------------------- = -------------
    //  totalRadialRateOfChange  * B   totalRotation
    //
    // So we have that:
    //
    //      totalDistance   totalRadialRateOfChange
    //  A = ------------- * ------------------------ * B
    //      totalRotation   totalForwardRateOfChange
    //
    // Now if we take the radius of the arc to be 1m, we have that:
    //
    //      totalRadialRateOfChange
    //  A = ------------------------ * B
    //      totalForwardRateOfChange
    //
    // Then we can just choose B = 1.0 and solve for A

    double B = 1.0;
    double A = (
        totalRadialRateOfChange.getRadiansPerSecond() /
        totalForwardRateOfChange.getMetersPerSecond()
    );

    m_unitForwardComponent = A;
    m_unitTurnComponent = B;
}

std::pair<double, double> CurveTurnFactorCalculator::getCurveTurnFactors(const Meters& radius) {
    return std::make_pair(m_unitForwardComponent * radius.getMeters(), m_unitTurnComponent);
}

} // namespace sim

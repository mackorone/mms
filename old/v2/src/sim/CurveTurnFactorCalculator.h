#pragma once

#include "Wheel.h"
#include "WheelEffect.h"

namespace sim {

class CurveTurnFactorCalculator {

public:
    CurveTurnFactorCalculator();
    CurveTurnFactorCalculator(
        const std::map<std::string, Wheel>& wheels,
        const std::map<std::string, WheelEffect>& wheelEffects,
        const std::map<std::string, std::pair<double, double>>& wheelSpeedAdjustmentFactors);

    // Returns a linear combination of forward and turn movement components
    // such that the mouse turns along the arc with the given radius. Note that
    // these factors are not necessarily between [-1.0, 1.0]
    std::pair<double, double> getCurveTurnFactors(const Meters& radius);

private:
    // The components if the desired radius was 1m (hence "unit")
    double m_unitForwardComponent;
    double m_unitTurnComponent;

};

} // namespace sim

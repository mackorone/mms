#pragma once

#include <QMap>
#include <QString>

#include "Wheel.h"
#include "WheelEffect.h"

namespace sim {

class CurveTurnFactorCalculator {

public:
    CurveTurnFactorCalculator();
    CurveTurnFactorCalculator(
        const QMap<QString, Wheel>& wheels,
        const QMap<QString, WheelEffect>& wheelEffects,
        const QMap<QString, std::pair<double, double>>& wheelSpeedAdjustmentFactors);

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

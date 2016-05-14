#pragma once

#include "Duration.h"
#include "Meters.h"
#include "Speed.h"

namespace sim {

class MetersPerSecond : public Speed {

public:
    MetersPerSecond();
    MetersPerSecond(double metersPerSecond);
    MetersPerSecond(const Speed& speed);
    MetersPerSecond operator+(const Speed& speed) const;
    MetersPerSecond operator-(const Speed& speed) const;
    MetersPerSecond operator*(double factor) const;
    MetersPerSecond operator/(double factor) const;
    double operator/(const Speed& speed) const;
    Meters operator*(const Duration& duration) const;
    void operator+=(const Speed& speed);

};

} // namespace sim

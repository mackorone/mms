#pragma once

#include "Speed.h"

namespace sim {

class MetersPerSecond : public Speed {

public:
    MetersPerSecond(double metersPerSecond);
    MetersPerSecond(const Speed& speed);
    MetersPerSecond operator+(const Speed& speed) const;
    MetersPerSecond operator-(const Speed& speed) const;

};

} // namespace sim

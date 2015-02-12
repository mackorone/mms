#pragma once

#include "Angle.h"

namespace sim {

class Radians : public Angle {

public:
    // TODO: Restrict it to [0,2pi]
    Radians(float radians);
    Radians(const Angle& angle);
    Radians operator+(const Angle& angle) const;

};

} // namespace sim

#pragma once

#include "Angle.h"

namespace sim {

class Radians : public Angle {

public:
    Radians(float radians);
    Radians(const Angle& angle);
    Radians operator+(const Angle& angle) const;

};

} // namespace sim

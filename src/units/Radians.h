#pragma once

#include "Angle.h"

namespace sim {

class Radians : public Angle {

public:
    Radians();
    Radians(float radians);
    Radians(const Angle& angle);
    Radians operator+(const Angle& angle) const;
    void operator+=(const Angle& angle);

};

} // namespace sim

#pragma once

#include "Angle.h"

namespace sim {

class Degrees : public Angle {

public:
    Degrees();
    Degrees(float degrees);
    Degrees(const Angle& angle);
    Degrees operator+(const Angle& angle) const;
    Degrees operator-(const Angle& angle) const;
    Degrees operator*(float factor) const;

};

} // namespace sim

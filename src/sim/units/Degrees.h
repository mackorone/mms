#pragma once

#include "Angle.h"

namespace sim {

class Degrees : public Angle {

public:
    Degrees();
    Degrees(double degrees);
    Degrees(const Angle& angle);

    Degrees operator+(const Angle& angle) const;
    Degrees operator-(const Angle& angle) const;
    Degrees operator*(double factor) const;
    Degrees operator/(double factor) const;

};

} // namespace sim

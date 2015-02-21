#pragma once

#include "Angle.h"

namespace sim {

class Degrees : public Angle {

public:
    Degrees(float degrees);
    Degrees(const Angle& angle);
    Degrees operator+(const Angle& angle) const;

};

} // namespace sim

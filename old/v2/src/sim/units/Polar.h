#pragma once

#include "Coordinate.h"

namespace sim {

class Polar : public Coordinate {

public:
    Polar(const Distance& rho, const Angle& theta);
    Polar(const Coordinate& coordinate);
    Polar operator+(const Coordinate& coordinate) const;

};

} // namespace sim

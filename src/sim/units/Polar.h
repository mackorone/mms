#pragma once

#include "Coordinate.h"

namespace sim {

class Polar : public Coordinate {

public:
    Polar(float rho, float theta);
    Polar(const Coordinate& coordinate);
    Polar operator+(const Coordinate& coordinate) const;

};

} // namespace sim

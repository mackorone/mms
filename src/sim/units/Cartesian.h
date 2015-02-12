#pragma once

#include "Coordinate.h"

namespace sim {

class Cartesian : public Coordinate {

public:
    Cartesian(float x, float y);
    Cartesian(const Coordinate& coordinate);
    Cartesian operator+(const Coordinate& coordinate) const;

};

} // namespace sim

#pragma once

#include "Coordinate.h"

namespace sim {

class Cartesian : public Coordinate {

public:
    Cartesian(const Distance& x, const Distance& y);
    Cartesian(const Coordinate& coordinate);
    Cartesian operator+(const Coordinate& coordinate) const;
    void operator+=(const Coordinate& coordinate);

};

} // namespace sim

#pragma once

#include "Coordinate.h"

namespace sim {

class Cartesian : public Coordinate {

public:
    Cartesian();
    Cartesian(const Distance& x, const Distance& y);
    Cartesian(const Coordinate& coordinate);
    Cartesian operator+(const Coordinate& coordinate) const;
    Cartesian operator-(const Coordinate& coordinate) const;
    Cartesian operator*(double factor) const;
    Cartesian operator/(double factor) const;
    void operator+=(const Coordinate& coordinate);

};

} // namespace sim

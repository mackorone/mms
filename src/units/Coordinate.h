#pragma once

#include "Meters.h"
#include "Radians.h"

namespace sim {

class Coordinate { // Synonymous to vector

public:
    virtual ~Coordinate() = 0;
    Meters getX() const;
    Meters getY() const;
    Meters getRho() const;
    Radians getTheta() const;
    bool operator==(const Coordinate& coordinate);
    bool operator<(const Coordinate& coordinate);

protected:
    Coordinate();
    Meters m_x;
    Meters m_y;

};

} // namespace sim

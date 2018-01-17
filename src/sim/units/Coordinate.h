#pragma once

#include "Angle.h"
#include "Meters.h"

namespace mms {

class Coordinate {

public:
    virtual ~Coordinate() = 0;
    Meters getX() const;
    Meters getY() const;
    Meters getRho() const;
    Angle getTheta() const;
    bool operator==(const Coordinate& coordinate) const;
    bool operator!=(const Coordinate& coordinate) const;
    bool operator<(const Coordinate& coordinate) const;

protected:
    Coordinate();
    Meters m_x;
    Meters m_y;

};

} // namespace mms

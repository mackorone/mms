#pragma once

#include "Angle.h"
#include "Meters.h"

namespace mms {

class Coordinate {

public:

    Coordinate();
    static Coordinate Cartesian(const Distance& x, const Distance& y);
    static Coordinate Polar(const Distance& rho, const Angle& theta);

    Meters getX() const;
    Meters getY() const;
    Meters getRho() const;
    Angle getTheta() const;

    Coordinate operator*(double factor) const;
    Coordinate operator/(double factor) const;
    Coordinate operator+(const Coordinate& other) const;
    Coordinate operator-(const Coordinate& other) const;
    bool operator==(const Coordinate& other) const;
    bool operator!=(const Coordinate& other) const;
    bool operator<(const Coordinate& other) const;
    void operator+=(const Coordinate& other);

private:

    Meters m_x;
    Meters m_y;
    Coordinate(const Distance& x, const Distance& y);

};

} // namespace mms

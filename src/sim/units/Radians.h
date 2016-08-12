#pragma once

#include "Angle.h"

namespace mms {

class Radians : public Angle {

public:
    Radians();
    Radians(double radians);
    Radians(const Angle& angle);
    Radians operator+(const Angle& angle) const;
    Radians operator-(const Angle& angle) const;
    Radians operator*(double factor) const;
    Radians operator/(double factor) const;
    void operator+=(const Angle& angle);
    void operator-=(const Angle& angle);

};

} // namespace mms

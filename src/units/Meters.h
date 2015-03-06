#pragma once

#include "Distance.h"

namespace sim {

class Meters : public Distance {

public:
    Meters();
    Meters(float meters);
    Meters(const Distance& distance);
    Meters operator+(const Distance& distance) const;
    Meters operator-(const Distance& distance) const;
    Meters operator*(float factor) const;
    Meters operator/(float factor) const;
    void operator+=(const Distance& distance);

};

} // namespace sim

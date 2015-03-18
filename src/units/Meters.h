#pragma once

#include "Distance.h"
#include "MetersSquared.h"

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
    float operator/(const Distance& distance) const;
    MetersSquared operator*(const Distance& distance) const;
    void operator+=(const Distance& distance);

};

} // namespace sim

#pragma once

#include "Distance.h"

namespace sim {

class Centimeters : public Distance {

public:
    Centimeters(float centimeters);
    Centimeters(const Distance& distance);
    Centimeters operator+(const Distance& distance) const;

};

} // namespace sim

#pragma once

#include "Distance.h"

namespace sim {

class Meters : public Distance {

public:
    Meters(float meters);
    Meters(const Distance& distance);
    Meters operator+(const Distance& distance) const;

};

} // namespace sim

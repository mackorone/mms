#pragma once

#include "Speed.h"

namespace sim {

// TODO: I shouldn't need this class
class MetersPerSecond : public Speed {

public:
    MetersPerSecond(float metersPerSecond);
    MetersPerSecond(const Speed& speed);
    MetersPerSecond operator+(const Speed& speed) const;

};

} // namespace sim

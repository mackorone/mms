#pragma once

#include "AngularVelocity.h"

namespace sim {

class DegreesPerSecond : public AngularVelocity {

public:
    DegreesPerSecond(double radiansPerSecond);
    DegreesPerSecond(const AngularVelocity& angularVelocity);

};

} // namespace sim

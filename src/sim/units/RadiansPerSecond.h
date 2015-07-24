#pragma once

#include "AngularVelocity.h"

namespace sim {

class RadiansPerSecond : public AngularVelocity {

public:
    RadiansPerSecond(double radiansPerSecond);
    RadiansPerSecond(const AngularVelocity& angularVelocity);
    RadiansPerSecond operator+(const AngularVelocity& angularVelocity) const;

};

} // namespace sim

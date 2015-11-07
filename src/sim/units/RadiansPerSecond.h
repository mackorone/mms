#pragma once

#include "AngularVelocity.h"
#include "Duration.h"
#include "Radians.h"

namespace sim {

class RadiansPerSecond : public AngularVelocity {

public:
    RadiansPerSecond(double radiansPerSecond);
    RadiansPerSecond(const AngularVelocity& angularVelocity);
    RadiansPerSecond operator+(const AngularVelocity& angularVelocity) const;
    RadiansPerSecond operator/(double factor) const;
    Radians operator*(const Duration& duration) const;
    void operator+=(const AngularVelocity& angularVelocity);

};

} // namespace sim

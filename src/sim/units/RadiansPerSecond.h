#pragma once

#include "AngularVelocity.h"
#include "Duration.h"
#include "MetersPerSecond.h"
#include "Radians.h"

namespace sim {

class RadiansPerSecond : public AngularVelocity {

public:
    RadiansPerSecond();
    RadiansPerSecond(double radiansPerSecond);
    RadiansPerSecond(const AngularVelocity& angularVelocity);
    RadiansPerSecond operator+(const AngularVelocity& angularVelocity) const;
    RadiansPerSecond operator*(double factor) const;
    RadiansPerSecond operator/(double factor) const;
    double operator/(const AngularVelocity& angularVelocity) const;
    Radians operator*(const Duration& duration) const;
    MetersPerSecond operator*(const Distance& radius) const;
    void operator+=(const AngularVelocity& angularVelocity);

};

} // namespace sim

#pragma once

#include "Duration.h"
#include "Radians.h"

namespace sim {

class AngularVelocity {

public:
    virtual ~AngularVelocity() = 0;
    double getRadiansPerSecond() const;
    double getDegreesPerSecond() const;
    double getRevolutionsPerMinute() const;
    Radians operator*(const Duration& duration) const;
    double operator/(const AngularVelocity& angularVelocity) const;
    bool operator<(const AngularVelocity& angularVelocity) const;
    bool operator<=(const AngularVelocity& angularVelocity) const;

protected:
    AngularVelocity();
    double m_radiansPerSecond;

};

} // namespace sim

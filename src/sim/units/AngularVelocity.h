#pragma once

#include "Duration.h"
#include "Radians.h"

namespace sim {

class AngularVelocity {

public:
    virtual ~AngularVelocity() = 0;
    double getRadiansPerSecond() const;
    double getDegreesPerSecond() const;
    Radians operator*(const Duration& duration) const;

protected:
    AngularVelocity();
    double m_radiansPerSecond;

};

} // namespace sim

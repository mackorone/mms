#pragma once

#include "Angle.h"
#include "Duration.h"

namespace mms {

class AngularVelocity {

public:
    virtual ~AngularVelocity() = 0;
    double getRadiansPerSecond() const;
    double getDegreesPerSecond() const;
    double getRevolutionsPerMinute() const;
    Angle operator*(const Duration& duration) const;
    double operator/(const AngularVelocity& angularVelocity) const;
    bool operator<(const AngularVelocity& angularVelocity) const;
    bool operator<=(const AngularVelocity& angularVelocity) const;

protected:
    AngularVelocity();
    double m_radiansPerSecond;

};

} // namespace mms

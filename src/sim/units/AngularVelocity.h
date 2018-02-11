#pragma once

#include "Angle.h"
#include "Duration.h"
#include "Speed.h"

namespace mms {

class AngularVelocity {

public:

    AngularVelocity();
    static AngularVelocity DegreesPerSecond(double degreesPerSecond);
    static AngularVelocity RadiansPerSecond(double radiansPerSecond);
    static AngularVelocity RevolutionsPerMinute(double revolutionsPerMinute);

    double getRadiansPerSecond() const;
    double getDegreesPerSecond() const;
    double getRevolutionsPerMinute() const;

    AngularVelocity operator*(double factor) const;
    AngularVelocity operator/(double factor) const;
    AngularVelocity operator+(const AngularVelocity& other) const;
    AngularVelocity operator-(const AngularVelocity& other) const;
    Angle operator*(const Duration& duration) const;
    Speed operator*(const Distance& radius) const;
    void operator+=(const AngularVelocity& other);
    bool operator<(const AngularVelocity& other) const;
    bool operator<=(const AngularVelocity& other) const;

private:

    double m_radiansPerSecond;
    AngularVelocity(double radiansPerSecond);

};

} // namespace mms

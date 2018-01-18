#pragma once

#include "Distance.h"
#include "Duration.h"

namespace mms {

class Speed {

public:

    Speed();
    static Speed MetersPerSecond(double metersPerSecond);

    double getMetersPerSecond() const;

    Speed operator*(double factor) const;
    Speed operator/(double factor) const;
    Speed operator+(const Speed& other) const;
    Speed operator-(const Speed& other) const;
    Distance operator*(const Duration& duration) const;
    double operator/(const Speed& other) const;
    bool operator<(const Speed& other) const;
    void operator+=(const Speed& other);


private:

    double m_metersPerSecond;
    Speed(double metersPerSecond);

};

} // namespace mms

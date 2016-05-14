#pragma once

#include "Seconds.h"
#include "Speed.h"

namespace sim {

class Distance {

public:
    virtual ~Distance() = 0;
    double getMeters() const;
    double getCentimeters() const;
    bool operator==(const Distance& distance) const;
    bool operator!=(const Distance& distance) const;
    bool operator<(const Distance& distance) const;
    Seconds operator/(const Speed& speed) const;

protected:
    Distance();
    double m_meters;

};

} // namespace sim

#pragma once

#include "Seconds.h"
#include "Speed.h"

namespace sim {

class Distance {

public:
    virtual ~Distance() = 0;
    float getMeters() const;
    float getCentimeters() const;
    bool operator==(const Distance& distance) const;
    bool operator!=(const Distance& distance) const;
    bool operator<(const Distance& distance) const;
    Seconds operator/(const Speed& speed) const;

protected:
    Distance();
    float m_meters;

};

} // namespace sim

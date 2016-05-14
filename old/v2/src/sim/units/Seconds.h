#pragma once

#include "Duration.h"

namespace sim {

class Seconds : public Duration {

public:
    Seconds();
    Seconds(double seconds);
    Seconds(const Duration& duration);
    Seconds operator+(const Duration& duration) const;
    Seconds operator-(const Duration& duration) const;
    Seconds operator*(double factor) const;
    void operator+=(const Duration& duration);

};

} // namespace sim

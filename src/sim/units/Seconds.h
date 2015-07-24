#pragma once

#include "Duration.h"

namespace sim {

class Seconds : public Duration {

public:
    Seconds(double seconds);
    Seconds(const Duration& duration);
    Seconds operator+(const Duration& duration) const;

};

} // namespace sim

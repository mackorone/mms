#pragma once

#include "Duration.h"

namespace sim {

class Seconds : public Duration {

public:
    Seconds(float seconds);
    Seconds(const Duration& duration);
    Seconds operator+(const Duration& duration) const;

};

} // namespace sim

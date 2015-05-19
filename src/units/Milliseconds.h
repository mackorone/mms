#pragma once

#include "Duration.h"

namespace sim {

class Milliseconds : public Duration {

public:
    Milliseconds(float milliseconds);
    Milliseconds(const Duration& duration);
    Milliseconds operator+(const Duration& duration) const;

};

} // namespace sim

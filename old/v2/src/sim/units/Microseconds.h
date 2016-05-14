#pragma once

#include "Duration.h"

namespace sim {

class Microseconds : public Duration {

public:
    Microseconds(double microseconds);
    Microseconds(const Duration& duration);

};

} // namespace sim

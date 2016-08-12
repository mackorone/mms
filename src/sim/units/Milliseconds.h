#pragma once

#include "Duration.h"

namespace mms {

class Milliseconds : public Duration {

public:
    Milliseconds(double milliseconds);
    Milliseconds(const Duration& duration);
    Milliseconds operator+(const Duration& duration) const;

};

} // namespace mms

#pragma once

#include "Duration.h"

namespace mms {

class Microseconds : public Duration {

public:
    Microseconds(double microseconds);
    Microseconds(const Duration& duration);

};

} // namespace mms

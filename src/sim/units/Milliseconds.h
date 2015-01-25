#pragma once

#include "Time.h"

namespace sim {

class Milliseconds : public Time {

public:
    Milliseconds(float milliseconds);
    Milliseconds(const Time& time);
    Milliseconds operator+(const Time& time) const;

};

} // namespace sim

#pragma once

#include "TTime.h"

namespace sim {

class Seconds : public Time {

public:
    Seconds(float seconds);
    Seconds(const Time& time);
    Seconds operator+(const Time& time) const;

};

} // namespace sim

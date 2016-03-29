#pragma once

#include "units/Seconds.h"

namespace sim {

// Wrapper for the static call to Time::getInstance();
class Time;
Time* T();

class Time {

public:

    // Returns a pointer to the singleton Time object
    static Time* getInstance();

    Seconds startTimestamp();
    Seconds elapsedRealTime();
    Seconds elapsedSimTime();

    void incrementElapsedSimTime(const Duration& duration);

private:

    // A private constructor is used to ensure only one instance of this class exists
    Time();

    // A pointer to the actual instance of the class
    static Time* INSTANCE;

    Seconds m_startTimestamp;
    Seconds m_elapsedSimTime;

};

} // namespace sim

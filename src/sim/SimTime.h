#pragma once

#include "units/Seconds.h"

namespace mms {

class SimTime {

public:

    static void init();
    static SimTime* get();

    Seconds startTimestamp();
    Seconds elapsedRealTime();
    Seconds elapsedSimTime();

    void incrementElapsedSimTime(const Duration& duration);

private:

    // A private constructor is used to ensure
    // only one instance of this class exists
    SimTime();

    // A pointer to the actual instance of the class
    static SimTime* INSTANCE;

    Seconds m_startTimestamp;
    Seconds m_elapsedSimTime;

};

} // namespace mms

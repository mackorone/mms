#pragma once

#include "units/Duration.h"

namespace mms {

class SimTime {

public:

    static void init();
    static SimTime* get();

    Duration startTimestamp();
    Duration elapsedRealTime();
    Duration elapsedSimTime();

    void incrementElapsedSimTime(const Duration& duration);
    void reset();

private:

    // A private constructor is used to ensure
    // only one instance of this class exists
    SimTime();

    // A pointer to the actual instance of the class
    static SimTime* INSTANCE;

    Duration m_startTimestamp;
    Duration m_elapsedSimTime;

};

} 

#include "Time.h"

#include "SimUtilities.h"

namespace sim {

Time* T() {
    return Time::getInstance();
}

// Definition of the variable for linking
Time* Time::INSTANCE = nullptr;
Time* Time::getInstance() {
    if (nullptr == INSTANCE) {
        INSTANCE = new Time();
    }
    return INSTANCE;
}

Time::Time() :
    m_startTimestamp(Seconds(SimUtilities::getHighResTimestamp())),
    m_elapsedSimTime(Seconds(0)) {
}

Seconds Time::startTimestamp() {
    return m_startTimestamp;
}

Seconds Time::elapsedRealTime() {
    return Seconds(SimUtilities::getHighResTimestamp()) - m_startTimestamp;
}

Seconds Time::elapsedSimTime() {
    return m_elapsedSimTime;
}

void Time::incrementElapsedSimTime(const Duration& duration) {
    m_elapsedSimTime += duration;
}

} // namespace sim

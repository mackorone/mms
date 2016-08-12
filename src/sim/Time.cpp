#include "Time.h"

#include "Assert.h"
#include "SimUtilities.h"

namespace mms {

Time* Time::INSTANCE = nullptr;

void Time::init() {
    SIM_ASSERT_TR(INSTANCE == nullptr);
    INSTANCE = new Time();
}

Time* Time::get() {
    SIM_ASSERT_FA(INSTANCE == nullptr);
    return INSTANCE;
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

Time::Time() :
    m_startTimestamp(Seconds(SimUtilities::getHighResTimestamp())),
    m_elapsedSimTime(Seconds(0)) {
}

} // namespace mms

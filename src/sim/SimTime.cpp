#include "SimTime.h"

#include "Assert.h"
#include "SimUtilities.h"

namespace mms {

SimTime* SimTime::INSTANCE = nullptr;

void SimTime::init() {
    ASSERT_TR(INSTANCE == nullptr);
    INSTANCE = new SimTime();
}

SimTime* SimTime::get() {
    ASSERT_FA(INSTANCE == nullptr);
    return INSTANCE;
}

Seconds SimTime::startTimestamp() {
    return m_startTimestamp;
}

Seconds SimTime::elapsedRealTime() {
    return Seconds(SimUtilities::getHighResTimestamp()) - m_startTimestamp;
}

Seconds SimTime::elapsedSimTime() {
    return m_elapsedSimTime;
}

void SimTime::incrementElapsedSimTime(const Duration& duration) {
    m_elapsedSimTime += duration;
}

SimTime::SimTime() :
    m_startTimestamp(Seconds(SimUtilities::getHighResTimestamp())),
    m_elapsedSimTime(Seconds(0)) {
}

} // namespace mms

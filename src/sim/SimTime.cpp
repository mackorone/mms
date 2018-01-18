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

Duration SimTime::startTimestamp() {
    return m_startTimestamp;
}

Duration SimTime::elapsedRealTime() {
    return Duration::Seconds(SimUtilities::getHighResTimestamp()) - m_startTimestamp;
}

Duration SimTime::elapsedSimTime() {
    return m_elapsedSimTime;
}

void SimTime::incrementElapsedSimTime(const Duration& duration) {
    m_elapsedSimTime += duration;
}

void SimTime::reset() {
    m_startTimestamp = Duration::Seconds(SimUtilities::getHighResTimestamp());
    m_elapsedSimTime = Duration::Seconds(0);
}

SimTime::SimTime() {
    reset();
}

} // namespace mms

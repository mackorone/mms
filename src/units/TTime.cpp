#include "TTime.h"

namespace sim {

Time::Time() : m_seconds(0) {
}

Time::~Time() {
}

float Time::getSeconds() const {
    return m_seconds;
}

float Time::getMilliseconds() const {
    return getSeconds() * 1000;
}

float Time::getMicroseconds() const {
    return getSeconds() * 1000 * 1000;
}

} // namespace sim

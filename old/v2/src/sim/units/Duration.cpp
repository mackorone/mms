#include "Duration.h"

namespace sim {

Duration::Duration() : m_seconds(0) {
}

Duration::~Duration() {
}

double Duration::getSeconds() const {
    return m_seconds;
}

double Duration::getMilliseconds() const {
    static const double millisecondsPerSecond = 1000.0;
    return millisecondsPerSecond * getSeconds();
}

double Duration::getMicroseconds() const {
    static const double microsecondsPerSecond = 1000.0 * 1000.0;
    return microsecondsPerSecond * getSeconds();
}

bool Duration::operator<(const Duration& duration) const {
    return getSeconds() < duration.getSeconds();
}

} // namespace sim

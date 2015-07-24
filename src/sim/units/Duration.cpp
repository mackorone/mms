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
    return getSeconds() * 1000;
}

double Duration::getMicroseconds() const {
    return getSeconds() * 1000 * 1000;
}

bool Duration::operator<(const Duration& duration) const {
    return getSeconds() < duration.getSeconds();
}

} // namespace sim

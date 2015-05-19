#include "Duration.h"

namespace sim {

Duration::Duration() : m_seconds(0) {
}

Duration::~Duration() {
}

float Duration::getSeconds() const {
    return m_seconds;
}

float Duration::getMilliseconds() const {
    return getSeconds() * 1000;
}

float Duration::getMicroseconds() const {
    return getSeconds() * 1000 * 1000;
}

} // namespace sim

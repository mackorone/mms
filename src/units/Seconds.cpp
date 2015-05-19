#include "Seconds.h"

namespace sim {

Seconds::Seconds(float seconds) {
    m_seconds = seconds;
}

Seconds::Seconds(const Duration& duration) {
    m_seconds = duration.getSeconds();
}

Seconds Seconds::operator+(const Duration& duration) const {
    return Seconds(getSeconds() + duration.getSeconds());
}

} // namespace sim

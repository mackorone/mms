#include "Seconds.h"

namespace sim {

Seconds::Seconds(float seconds) {
    m_seconds = seconds;
}

Seconds::Seconds(const Time& time) {
    m_seconds = time.getSeconds();
}

Seconds Seconds::operator+(const Time& time) const {
    return Seconds(getSeconds() + time.getSeconds());
}

} // namespace sim

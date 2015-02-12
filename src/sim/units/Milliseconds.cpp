#include "Milliseconds.h"

namespace sim {

Milliseconds::Milliseconds(float milliseconds) {
    m_seconds = milliseconds / 1000.0;
}

Milliseconds::Milliseconds(const Time& time) {
    m_seconds = time.getSeconds();
}

Milliseconds Milliseconds::operator+(const Time& time) const {
    return Milliseconds(getMilliseconds() + time.getMilliseconds());
}

} // namespace sim

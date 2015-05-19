#include "Milliseconds.h"

namespace sim {

Milliseconds::Milliseconds(float milliseconds) {
    m_seconds = milliseconds / 1000.0;
}

Milliseconds::Milliseconds(const Duration& duration) {
    m_seconds = duration.getSeconds();
}

Milliseconds Milliseconds::operator+(const Duration& duration) const {
    return Milliseconds(getMilliseconds() + duration.getMilliseconds());
}

} // namespace sim

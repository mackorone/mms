#include "Milliseconds.h"

namespace mms {

Milliseconds::Milliseconds(double milliseconds) {
    static const double secondsPerMillisecond = 1.0 / 1000.0;
    m_seconds = secondsPerMillisecond * milliseconds;
}

Milliseconds::Milliseconds(const Duration& duration) {
    m_seconds = duration.getSeconds();
}

Milliseconds Milliseconds::operator+(const Duration& duration) const {
    return Milliseconds(getMilliseconds() + duration.getMilliseconds());
}

} // namespace mms

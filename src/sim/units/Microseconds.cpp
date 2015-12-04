#include "Microseconds.h"

namespace sim {

Microseconds::Microseconds(double microseconds) {
    static const double secondsPerMicrosecond = 1.0 / 1000.0 / 1000.0;
    m_seconds = secondsPerMicrosecond * microseconds;
}

Microseconds::Microseconds(const Duration& duration) {
    m_seconds = duration.getSeconds();
}

} // namespace sim

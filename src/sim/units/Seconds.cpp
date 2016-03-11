#include "Seconds.h"

namespace sim {

Seconds::Seconds() {
    m_seconds = 0.0;
}

Seconds::Seconds(double seconds) {
    m_seconds = seconds;
}

Seconds::Seconds(const Duration& duration) {
    m_seconds = duration.getSeconds();
}

Seconds Seconds::operator+(const Duration& duration) const {
    return Seconds(getSeconds() + duration.getSeconds());
}

Seconds Seconds::operator-(const Duration& duration) const {
    return Seconds(getSeconds() - duration.getSeconds());
}

Seconds Seconds::operator*(double factor) const {
    return Seconds(getSeconds() * factor);
}

void Seconds::operator+=(const Duration& duration) {
    m_seconds += duration.getSeconds();
}

} // namespace sim

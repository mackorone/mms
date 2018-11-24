#include "Duration.h"

namespace mms {

Duration::Duration() : Duration(0.0) {
}

Duration Duration::Seconds(double seconds) {
    return Duration(seconds);
}

Duration Duration::Milliseconds(double milliseconds) {
    static const double secondsPerMillisecond = 1.0 / 1000.0;
    return Duration(secondsPerMillisecond * milliseconds);
}

Duration Duration::Microseconds(double microseconds) {
    static const double secondsPerMicrosecond = 1.0 / 1000.0 / 1000.0;
    return Duration(secondsPerMicrosecond * microseconds);
}

double Duration::getSeconds() const {
    return m_seconds;
}

double Duration::getMilliseconds() const {
    static const double millisecondsPerSecond = 1000.0;
    return millisecondsPerSecond * m_seconds;
}

double Duration::getMicroseconds() const {
    static const double microsecondsPerSecond = 1000.0 * 1000.0;
    return microsecondsPerSecond * m_seconds;
}

Duration Duration::operator*(double factor) const {
    return Duration(m_seconds * factor);
}

Duration Duration::operator+(const Duration& other) const {
    return Duration(m_seconds + other.m_seconds);
}

Duration Duration::operator-(const Duration& other) const {
    return Duration(m_seconds - other.m_seconds);
}

bool Duration::operator<(const Duration& other) const {
    return m_seconds < other.m_seconds;
}

void Duration::operator+=(const Duration& other) {
    m_seconds += other.m_seconds;
}

Duration::Duration(double seconds) : m_seconds(seconds) {
}

} 

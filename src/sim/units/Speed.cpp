#include "Speed.h"

#include "../Assert.h"

namespace mms {

Speed::Speed() : Speed(0.0) {
}

Speed Speed::MetersPerSecond(double metersPerSecond) {
    return Speed(metersPerSecond);
}

double Speed::getMetersPerSecond() const {
    return m_metersPerSecond;
}

Speed Speed::operator*(double factor) const {
    return Speed(m_metersPerSecond * factor);
}

Speed Speed::operator/(double factor) const {
    ASSERT_NE(factor, 0.0);
    return Speed(m_metersPerSecond / factor);
}

Speed Speed::operator+(const Speed& other) const {
    return Speed(m_metersPerSecond + other.m_metersPerSecond);
}

Speed Speed::operator-(const Speed& other) const {
    return Speed(m_metersPerSecond - other.m_metersPerSecond);
}

Distance Speed::operator*(const Duration& duration) const {
    return Distance::Meters(m_metersPerSecond * duration.getSeconds());
}

double Speed::operator/(const Speed& other) const {
    ASSERT_NE(other.m_metersPerSecond, 0.0);
    return m_metersPerSecond / other.m_metersPerSecond;
}

bool Speed::operator<(const Speed& other) const {
    return m_metersPerSecond < other.m_metersPerSecond;
}

void Speed::operator+=(const Speed& other) {
    m_metersPerSecond += other.getMetersPerSecond();
}

Speed::Speed(double metersPerSecond) : m_metersPerSecond(metersPerSecond) {
}

} // namespace mms

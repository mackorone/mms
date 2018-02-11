#include "AngularVelocity.h"

#include <QtMath>

#include "../Assert.h"

namespace mms {

AngularVelocity::AngularVelocity() : AngularVelocity(0.0) {
}

AngularVelocity AngularVelocity::RadiansPerSecond(double radiansPerSecond) {
    return AngularVelocity(radiansPerSecond);
}

AngularVelocity AngularVelocity::DegreesPerSecond(double degreesPerSecond) {
    static const double radiansPerDegree = 2 * M_PI / 360.0;
    return AngularVelocity(radiansPerDegree * degreesPerSecond);
}

AngularVelocity AngularVelocity::RevolutionsPerMinute(
    double revolutionsPerMinute) {
    static const double minutesPerSecond = 1.0 / 60.0;
    static const double radiansPerRevolution = 2 * M_PI;
    return AngularVelocity(
        radiansPerRevolution * revolutionsPerMinute * minutesPerSecond);
}

double AngularVelocity::getRadiansPerSecond() const {
    return m_radiansPerSecond;
}

double AngularVelocity::getDegreesPerSecond() const {
    static const double degreesPerRadian = 360.0 / (2 * M_PI);
    return degreesPerRadian * m_radiansPerSecond;
}

double AngularVelocity::getRevolutionsPerMinute() const {
    static const double revolutionsPerRadian = 1.0 / (2 * M_PI);
    static const double secondsPerMinute = 60.0;
    return revolutionsPerRadian * m_radiansPerSecond * secondsPerMinute;
}

AngularVelocity AngularVelocity::operator*(double factor) const {
    return AngularVelocity(m_radiansPerSecond * factor);
}

AngularVelocity AngularVelocity::operator/(double factor) const {
    ASSERT_NE(factor, 0.0);
    return AngularVelocity(m_radiansPerSecond / factor);
}

AngularVelocity AngularVelocity::operator+(const AngularVelocity& other) const {
    return AngularVelocity(m_radiansPerSecond + other.m_radiansPerSecond);
}

AngularVelocity AngularVelocity::operator-(const AngularVelocity& other) const {
    return AngularVelocity(m_radiansPerSecond - other.m_radiansPerSecond);
}

Angle AngularVelocity::operator*(const Duration& duration) const {
    return Angle::Radians(m_radiansPerSecond * duration.getSeconds());
}

Speed AngularVelocity::operator*(const Distance& radius) const {
    return Speed::MetersPerSecond(m_radiansPerSecond * radius.getMeters());
}

void AngularVelocity::operator+=(const AngularVelocity& other) {
    m_radiansPerSecond += other.m_radiansPerSecond;
}

bool AngularVelocity::operator<(const AngularVelocity& other) const {
    return m_radiansPerSecond < other.m_radiansPerSecond;
}

bool AngularVelocity::operator<=(const AngularVelocity& other) const {
    return m_radiansPerSecond <= other.m_radiansPerSecond;
}

AngularVelocity::AngularVelocity(double radiansPerSecond) :
    m_radiansPerSecond(radiansPerSecond) {
}

} // namespace mms

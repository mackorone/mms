#include "Angle.h"

#include <QtMath>

#include "../AssertMacros.h"

namespace mms {

Angle::Angle() : Angle(0.0) {}

Angle Angle::Radians(double radians) { return Angle(radians); }

Angle Angle::Degrees(double degrees) {
  static const double radiansPerDegree = 2 * M_PI / 360.0;
  return Angle(radiansPerDegree * degrees);
}

double Angle::getRadiansZeroTo2pi() const { return getRadians(true); }

double Angle::getDegreesZeroTo360() const { return getDegrees(true); }

double Angle::getRadiansUnbounded() const { return getRadians(false); }

double Angle::getDegreesUnbounded() const { return getDegrees(false); }

double Angle::getSin() const { return std::sin(getRadiansZeroTo2pi()); }

double Angle::getCos() const { return std::cos(getRadiansZeroTo2pi()); }

Angle Angle::operator*(double factor) const {
  return Angle(m_radians * factor);
}

Angle Angle::operator/(double factor) const {
  ASSERT_NE(factor, 0.0);
  return Angle(m_radians / factor);
}

Angle Angle::operator+(const Angle &other) const {
  return Angle(m_radians + other.m_radians);
}

Angle Angle::operator-(const Angle &other) const {
  return Angle(m_radians - other.m_radians);
}

void Angle::operator+=(const Angle &other) { m_radians += other.m_radians; }

void Angle::operator-=(const Angle &other) { m_radians -= other.m_radians; }

bool Angle::operator<(const Angle &other) const {
  return getRadiansZeroTo2pi() < other.getRadiansZeroTo2pi();
}

Angle::Angle(double radians) : m_radians(radians) {}

double Angle::getRadians(bool zeroTo2pi) const {
  double radians = m_radians;
  if (zeroTo2pi) {
    radians = std::fmod(radians, 2 * M_PI);
    if (radians < 0) {
      radians += 2 * M_PI;
    }
    if (2 * M_PI <= radians) {
      radians -= 2 * M_PI;
    }
    ASSERT_LE(0, radians);
    ASSERT_LT(radians, 2 * M_PI);
  }
  return radians;
}

double Angle::getDegrees(bool zeroTo360) const {
  static const double degreesPerRadian = 360.0 / (2 * M_PI);
  return degreesPerRadian * getRadians(zeroTo360);
}

}  // namespace mms

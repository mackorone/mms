#include "Coordinate.h"

#include <QtMath>

#include "../AssertMacros.h"

namespace mms {

Coordinate::Coordinate() : Coordinate(Distance(), Distance()) {}

Coordinate Coordinate::Cartesian(const Distance &x, const Distance &y) {
  return Coordinate(x, y);
}

Coordinate Coordinate::Polar(const Distance &rho, const Angle &theta) {
  return Coordinate(rho * theta.getCos(), rho * theta.getSin());
}

Distance Coordinate::getX() const { return m_x; }

Distance Coordinate::getY() const { return m_y; }

Distance Coordinate::getRho() const {
  return Distance::Meters(std::hypot(m_x.getMeters(), m_y.getMeters()));
}

Angle Coordinate::getTheta() const {
  return Angle::Radians(std::atan2(m_y.getMeters(), m_x.getMeters()));
}

Coordinate Coordinate::operator*(double factor) const {
  return Coordinate(m_x * factor, m_y * factor);
}

Coordinate Coordinate::operator/(double factor) const {
  ASSERT_NE(factor, 0.0);
  return Coordinate(m_x / factor, m_y / factor);
}

Coordinate Coordinate::operator+(const Coordinate &other) const {
  return Coordinate(m_x + other.m_x, m_y + other.m_y);
}

Coordinate Coordinate::operator-(const Coordinate &other) const {
  return Coordinate(m_x - other.m_x, m_y - other.m_y);
}

bool Coordinate::operator==(const Coordinate &other) const {
  return (m_x == other.m_x) && (m_y == other.m_y);
}

bool Coordinate::operator!=(const Coordinate &other) const {
  return (!operator==(other));
}

bool Coordinate::operator<(const Coordinate &other) const {
  return (m_x != other.m_x ? m_x < other.m_x : m_y < other.m_y);
}

void Coordinate::operator+=(const Coordinate &other) {
  m_x += other.m_x;
  m_y += other.m_y;
}

Coordinate::Coordinate(const Distance &x, const Distance &y) : m_x(x), m_y(y) {}

}  // namespace mms

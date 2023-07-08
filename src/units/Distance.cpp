#include "Distance.h"

#include "../AssertMacros.h"

namespace mms {

Distance::Distance() : Distance(0.0) {}

Distance Distance::Meters(double meters) { return Distance(meters); }

double Distance::getMeters() const { return m_meters; }

Distance Distance::operator*(double factor) const {
  return Distance(m_meters * factor);
}

Distance Distance::operator/(double factor) const {
  ASSERT_NE(factor, 0.0);
  return Distance(m_meters / factor);
}

Distance Distance::operator+(const Distance &other) const {
  return Distance(m_meters + other.m_meters);
}

Distance Distance::operator-(const Distance &other) const {
  return Distance(m_meters - other.m_meters);
}

double Distance::operator/(const Distance &other) const {
  ASSERT_NE(other.m_meters, 0.0);
  return m_meters / other.m_meters;
}

bool Distance::operator==(const Distance &other) const {
  return m_meters == other.m_meters;
}

bool Distance::operator!=(const Distance &other) const {
  return (!operator==(other));
}

bool Distance::operator<(const Distance &other) const {
  return m_meters < other.m_meters;
}

void Distance::operator+=(const Distance &other) { m_meters += other.m_meters; }

Distance::Distance(double meters) : m_meters(meters) {}

}  // namespace mms

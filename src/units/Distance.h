#pragma once

namespace mms {

class Distance {
 public:
  Distance();
  static Distance Meters(double meters);

  double getMeters() const;

  Distance operator*(double factor) const;
  Distance operator/(double factor) const;
  Distance operator+(const Distance &other) const;
  Distance operator-(const Distance &other) const;
  double operator/(const Distance &other) const;
  bool operator==(const Distance &other) const;
  bool operator!=(const Distance &other) const;
  bool operator<(const Distance &other) const;
  void operator+=(const Distance &other);

 private:
  double m_meters;
  Distance(double meters);
};

}  // namespace mms

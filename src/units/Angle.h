#pragma once

namespace mms {

class Angle {
 public:
  Angle();
  static Angle Radians(double radians);
  static Angle Degrees(double degrees);

  double getRadiansZeroTo2pi() const;
  double getDegreesZeroTo360() const;
  double getRadiansUnbounded() const;
  double getDegreesUnbounded() const;
  double getSin() const;
  double getCos() const;

  Angle operator*(double factor) const;
  Angle operator/(double factor) const;
  Angle operator+(const Angle &other) const;
  Angle operator-(const Angle &other) const;
  void operator+=(const Angle &other);
  void operator-=(const Angle &other);
  bool operator<(const Angle &other) const;

 private:
  double m_radians;
  Angle(double radians);
  double getRadians(bool zeroTo2pi) const;
  double getDegrees(bool zeroTo360) const;
};

}  // namespace mms

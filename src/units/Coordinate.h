#pragma once

#include "Angle.h"
#include "Distance.h"

namespace mms {

class Coordinate {
 public:
  Coordinate();
  static Coordinate Cartesian(const Distance &x, const Distance &y);
  static Coordinate Polar(const Distance &rho, const Angle &theta);

  Distance getX() const;
  Distance getY() const;
  Distance getRho() const;
  Angle getTheta() const;

  Coordinate operator*(double factor) const;
  Coordinate operator/(double factor) const;
  Coordinate operator+(const Coordinate &other) const;
  Coordinate operator-(const Coordinate &other) const;
  bool operator==(const Coordinate &other) const;
  bool operator!=(const Coordinate &other) const;
  bool operator<(const Coordinate &other) const;
  void operator+=(const Coordinate &other);

 private:
  Distance m_x;
  Distance m_y;
  Coordinate(const Distance &x, const Distance &y);
};

}  // namespace mms

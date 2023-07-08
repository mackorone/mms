#pragma once

#include "units/Distance.h"

namespace mms {

class Dimensions {
 public:
  Dimensions() = delete;
  static Distance tileLength();
  static Distance halfTileLength();
  static Distance quarterTileLength();
  static Distance wallLength();
  static Distance wallWidth();
  static Distance halfWallWidth();
};

}  // namespace mms

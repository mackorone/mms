#pragma once

#include "RGB.h"

namespace mms {

struct VertexGraphic {
  float x;          // x position
  float y;          // y position
  RGB rgb;          // rgb values
  unsigned char a;  // alpha value
};

}  // namespace mms

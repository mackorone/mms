#pragma once

#include "Constants.h"

namespace mackAlgoTwo {

typedef (Constants::NUM_CELLS <= 256 ? unsigned char : unsigned int) word;

} // namespace mackAlgoTwo

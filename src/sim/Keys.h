#pragma once

#include <vector>

namespace sim {

static const int SPACE =  32;

static const int LEFT  = 100;
static const int UP    = 101;
static const int RIGHT = 102;
static const int DOWN  = 103;

static const std::vector<int> ARROW_KEYS = { LEFT, UP, RIGHT, DOWN };

} // namespace sim

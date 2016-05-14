#pragma once

#include "Parameters.h"

namespace sim {

// Converts points from pixel coordinates to opengl specification
inline float convertHorizontalPoint(float coordinate) {
    return ((coordinate / WINDOW_WIDTH) - 0.5f) * 2;
}

// Converts points from pixel coordinates to opengl specification
inline float convertVerticalPoint(float coordinate) {
    return ((coordinate / WINDOW_HEIGHT) - 0.5f) * 2;
}

} // namespace sim

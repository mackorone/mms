#pragma once

namespace sim {

struct VertexTexture {
    double x; // x position // TODO: MACK - why are these doubles instead of floats??? How is this working...
    double y; // y position
    double u; // u position (x position in the texture)
    double v; // v position (y position in the texture)
};

} // namespace sim

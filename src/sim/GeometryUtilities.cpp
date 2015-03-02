#include "GeometryUtilities.h"

namespace sim {

// Stolen code
bool linesIntersect(std::pair<const Cartesian&, const Cartesian&> A,
                    std::pair<const Cartesian&, const Cartesian&> B) {

    // Output
    float i_x;
    float i_y;

    float p0_x = A.first.getX().getMeters();
    float p0_y = A.first.getY().getMeters();
    float p1_x = A.second.getX().getMeters();
    float p1_y = A.second.getY().getMeters();
    float p2_x = B.first.getX().getMeters();
    float p2_y = B.first.getY().getMeters();
    float p3_x = B.second.getX().getMeters();
    float p3_y = B.second.getY().getMeters();

    float s1_x = p1_x - p0_x;
    float s1_y = p1_y - p0_y;
    float s2_x = p3_x - p2_x;
    float s2_y = p3_y - p2_y;
    float s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    float t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);
    if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
        /*
        if (i_x != NULL) {
            i_x = p0_x + (t * s1_x);
        }
        if (i_y != NULL) {
            i_y = p0_y + (t * s1_y);
        }
        */
        return true;
    }
    return false;
}

// TODO: area of intersection of two polygons

} // namespace sim

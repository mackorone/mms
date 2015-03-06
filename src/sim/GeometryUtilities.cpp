#include "GeometryUtilities.h"

#include <cmath>

namespace sim {

bool linesIntersect(const std::pair<const Cartesian&, const Cartesian&>& A,
                    const std::pair<const Cartesian&, const Cartesian&>& B) {

    // Extract A's points
    float a1x = A.first.getX().getMeters();
    float a1y = A.first.getY().getMeters();
    float a2x = A.second.getX().getMeters();
    float a2y = A.second.getY().getMeters();

    // Extract B's points
    float b1x = B.first.getX().getMeters();
    float b1y = B.first.getY().getMeters();
    float b2x = B.second.getX().getMeters();
    float b2y = B.second.getY().getMeters();

    // Test to determine if the line segments cross eachother
    float t1 = (b2x-b1x)*(a1y-b2y) - (b2y-b1y)*(a1x-b2x);
    float t2 = (b2x-b1x)*(a2y-b2y) - (b2y-b1y)*(a2x-b2x);
    float t3 = (a2x-a1x)*(b1y-a2y) - (a2y-a1y)*(b1x-a2x);
    float t4 = (a2x-a1x)*(b2y-a2y) - (a2y-a1y)*(b2x-a2x);

    return (t1*t2 <= 0 && t3*t4 <=0);
}

MetersSquared polygonArea(const Polygon& polygon) {

    // Magic
    float area = 0;
    std::vector<Cartesian> vertices = polygon.getVertices();
    for (int i = 0; i < vertices.size(); i += 1) {
        int j = (i + 1) % vertices.size();
        area += vertices.at(i).getX().getMeters() * vertices.at(j).getY().getMeters();
        area -= vertices.at(i).getY().getMeters() * vertices.at(j).getX().getMeters();
    }
    area /= 2;

    return MetersSquared(std::abs(area));
}

// TODO: area of intersection of two polygons

} // namespace sim

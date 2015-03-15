#include "GeometryUtilities.h"

#include <cmath>

#include "Assert.h"

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

    return (t1*t2 <= 0 && t3*t4 <= 0);
}

Cartesian getIntersectionPoint(const std::pair<const Cartesian&, const Cartesian&>& A, 
                               const std::pair<const Cartesian&, const Cartesian&>& B) {

    // Assert that the lines do intersect
    ASSERT(linesIntersect(A, B));

    // Taken from http://alienryderflex.com/intersect/

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

    // Translate the system so that point A is on the origin.
    a2x -= a1x;
    a2y -= a1y;
    b1x -= a1x;
    b1y -= a1y;
    b2x -= a1x;
    b2y -= a1y;

    // Discover the length of segment A-B.
    float distAB = sqrt(a2x*a2x+a2y*a2y);

    // Rotate the system so that point B is on the positive X axis.
    float theCos = a2x / distAB;
    float theSin = a2y / distAB;
    float newX = b1x * theCos + b1y * theSin;
    b1y = b1y * theCos - b1x * theSin;
    b1x = newX;
    newX = b2x * theCos + b2y * theSin;
    b2y = b2y * theCos - b2x * theSin;
    b2x = newX;

    // Discover the position of the intersection point along line A-B.
    float ABpos = b2x + (b1x - b2x) * b2y / (b2y - b1y);

    // Apply the discovered position to line A-B in the original coordinate system.
    return Cartesian(Meters(a1x+ABpos*theCos), Meters(a1y+ABpos*theSin));
}

MetersSquared polygonArea(const Polygon& polygon) {

    // Magic
    float area = 0.0;
    std::vector<Cartesian> vertices = polygon.getVertices();
    for (int i = 0; i < vertices.size(); i += 1) {
        int j = (i + 1) % vertices.size();
        area += vertices.at(i).getX().getMeters() * vertices.at(j).getY().getMeters();
        area -= vertices.at(i).getY().getMeters() * vertices.at(j).getX().getMeters();
    }
    area /= 2.0;

    return MetersSquared(std::abs(area));
}

std::vector<std::pair<Cartesian, Cartesian>> getLineSegments(const Polygon& polygon) {

    std::vector<std::pair<Cartesian, Cartesian>> segments;

    std::vector<Cartesian> vertices = polygon.getVertices();
    Cartesian previousPoint = vertices.back();
    for (Cartesian currentPoint : vertices) {
        segments.push_back(std::make_pair(previousPoint, currentPoint));
        previousPoint = currentPoint;
    }

    return segments;
}

} // namespace sim

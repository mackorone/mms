#pragma once

#include "units/Angle.h"
#include "units/Cartesian.h"
#include "units/Distance.h"
#include "units/MetersSquared.h"

#include "Maze.h"
#include "Polygon.h"

namespace sim {

class GeometryUtilities {

public:

    // The GeometryUtilities class is not constructible
    GeometryUtilities() = delete;

    // Returns the result of translating the vertex by translation
    static Cartesian translateVertex(const Cartesian& vertex, const Coordinate& translation);

    // Returns the result of rotating the vertex around point by the amount specified by angle
    static Cartesian rotateVertexAroundPoint(const Cartesian& vertex, const Angle& angle, const Coordinate& point);

    // Creates a circle polygon
    static Polygon createCirclePolygon(const Cartesian& position, const Distance& radius, int numberOfEdges = 8);

    // Returns the cross product of the vectors ZA and ZB
    static MetersSquared crossProduct(const Cartesian& Z, const Cartesian& A, const Cartesian& B);

    // Returns true if the line segments A and B intersect
    static bool linesIntersect(const std::pair<const Cartesian&, const Cartesian&>& A,
                               const std::pair<const Cartesian&, const Cartesian&>& B);

    // Finds the point at which the line segments A and B intersect
    static Cartesian getIntersectionPoint(const std::pair<const Cartesian&, const Cartesian&>& A,
                                          const std::pair<const Cartesian&, const Cartesian&>& B);

    // Returns all of the tiles touched by the segment AB
    static std::vector<const Tile*> lineSegmentTileCover(const Cartesian& A, const Cartesian& B, const Maze& maze);

    // Returns the convex hull of all of the points of all of the polygons
    static Polygon convexHull(const std::vector<Polygon>& polygons);
};

} // namespace sim

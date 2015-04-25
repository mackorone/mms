#pragma once

#include <utility>

#include <Cartesian.h>
#include <MetersSquared.h>

#include "Maze.h"
#include "Polygon.h"

namespace sim {

class GeometryUtilities {

public:

    // Returns all of the tiles touched by the segment AB
    static std::vector<const Tile*> lineSegmentTileCover(const Cartesian& A, const Cartesian& B, const Maze& maze);

    // Returns true if the line segments A and B intersect
    static bool linesIntersect(const std::pair<const Cartesian&, const Cartesian&>& A,
                               const std::pair<const Cartesian&, const Cartesian&>& B);

    // Finds the point at which the line segments A and B intersect
    // Note: This should only be called if the lines actually intersect
    static Cartesian getIntersectionPoint(const std::pair<const Cartesian&, const Cartesian&>& A,
                                          const std::pair<const Cartesian&, const Cartesian&>& B);

    // Returns the area of a polygon
    static MetersSquared polygonArea(const Polygon& polygon);

    // Retrieves every pair of neighboring line segments in a polygon
    static std::vector<std::pair<Cartesian, Cartesian>> getLineSegments(const Polygon& polygon);

    // Returns the convex hull of all of the points of all of the polygons
    static Polygon convexHull(const std::vector<Polygon>& polygons);

private:

    // A private constructor to restrict creation of any GeometryUtilities objects
    GeometryUtilities();

    // Returns the cross product of the vectors ZA and ZB
    static MetersSquared crossProduct(const Cartesian& Z, const Cartesian& A, const Cartesian& B);

    // Returns the polygon that is the union all of the polygons
    static Polygon getUnionMultiple(const std::vector<Polygon>& polygons);
    static Polygon getUnionTwo(const Polygon& A, const Polygon& B); 

};

} // namespace sim

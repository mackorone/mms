#pragma once

#include <utility>

#include <Cartesian.h>
#include <MetersSquared.h>

#include "Maze.h"
#include "Polygon.h"

namespace sim {

// Returns all of the tiles touched by the segment AB
std::vector<const Tile*> lineSegmentTileCover(const Cartesian& A, const Cartesian& B, const Maze& maze);

// Returns the cross product of the vectors ZA and ZB
MetersSquared crossProduct(const Cartesian& Z, const Cartesian& A, const Cartesian& B);

// Returns the centroid of the polygon
Cartesian centroid(const Polygon& polygon);

// Returns true if the line segments A and B intersect
bool linesIntersect(const std::pair<const Cartesian&, const Cartesian&>& A,
                    const std::pair<const Cartesian&, const Cartesian&>& B);

// Finds the point at which the line segments A and B intersect
// Note: This should only be called if the lines actually intersect
Cartesian getIntersectionPoint(const std::pair<const Cartesian&, const Cartesian&>& A,
                               const std::pair<const Cartesian&, const Cartesian&>& B);

// Returns the area of a polygon
MetersSquared polygonArea(const Polygon& polygon);

// Retrieves every pair of neighboring line segments in a polygon
std::vector<std::pair<Cartesian, Cartesian>> getLineSegments(const Polygon& polygon);

// Returns the convex hull of all of the points of all of the polygons
Polygon convexHull(const std::vector<Polygon>& polygons);

// Returns the polygon that is the union all of the polygons
Polygon getUnionMultiple(const std::vector<Polygon>& polygons);

Polygon getUnionTwo(const Polygon& A, const Polygon& B); 


} // namespace sim

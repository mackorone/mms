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
    static Polygon createCirclePolygon(const Cartesian& position, const Distance& radius, int numberOfEdges);

    // Returns the cross product of the vectors ZA and ZB
    static MetersSquared crossProduct(const Cartesian& Z, const Cartesian& A, const Cartesian& B);

    // Returns the convex hull of all of the points of all of the polygons
    static Polygon convexHull(const std::vector<Polygon>& polygons);

    // Attempts to cast a ray from start to end and returns the first point of
    // intersection with walls or corners, as given by maze, or end if none
    static Cartesian castRay(
        const Cartesian& start, const Cartesian& end, const Maze& maze,
        const Meters& halfWallWidth, const Meters& tileLength);

    // Returns true if position is located on the edge of a tile, based on halfWallWidth and tileLength
    static bool isOnTileEdge(
        const Meters& position, const Meters& halfWallWidth, const Meters& tileLength);
};

} // namespace sim

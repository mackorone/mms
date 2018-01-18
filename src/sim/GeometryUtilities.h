#pragma once

#include "Maze.h"
#include "Polygon.h"
#include "units/Angle.h"
#include "units/Area.h"
#include "units/Coordinate.h"
#include "units/Distance.h"

namespace mms {

class GeometryUtilities {

public:

    // The GeometryUtilities class is not constructible
    GeometryUtilities() = delete;

    // Returns the result of translating the vertex by translation
    static Coordinate translateVertex(const Coordinate& vertex, const Coordinate& translation);

    // Returns the result of rotating the vertex around point by the amount specified by angle
    static Coordinate rotateVertexAroundPoint(const Coordinate& vertex, const Angle& angle, const Coordinate& point);

    // Creates a circle polygon
    static Polygon createCirclePolygon(const Coordinate& position, const Distance& radius, int numberOfEdges);

    // Returns the cross product of the vectors ZA and ZB
    static Area crossProduct(const Coordinate& Z, const Coordinate& A, const Coordinate& B);

    // Returns the convex hull of all of the points of all of the polygons
    static Polygon convexHull(const QVector<Polygon>& polygons);

    // Attempts to cast a ray from start to end and returns the first point of
    // intersection with walls or corners, as given by maze, or end if none
    static Coordinate castRay(
        const Coordinate& start,
        const Coordinate& end,
        const Maze& maze,
        const Distance& halfWallWidth,
        const Distance& tileLength);

    // Returns true if position is located on the edge of a tile, based on
    // halfWallWidth and tileLength
    static bool isOnTileEdge(
        const Distance& position,
        const Distance& halfWallWidth,
        const Distance& tileLength);
};

} // namespace mms

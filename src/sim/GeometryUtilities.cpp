#include "GeometryUtilities.h"

#include <algorithm>
#include <functional>

#include "Assert.h"
#include "CPMath.h"
#include "units/Polar.h"

namespace sim {

Cartesian GeometryUtilities::translateVertex(const Cartesian& vertex, const Coordinate& translation) {
    return vertex + translation;
}

Cartesian GeometryUtilities::rotateVertexAroundPoint(const Cartesian& vertex, const Angle& angle, const Coordinate& point) {
    Cartesian relativeVertex(
        vertex.getX() - point.getX(),
        vertex.getY() - point.getY()
    );
    Polar rotatedRelativeVertex(
        relativeVertex.getRho(),
        relativeVertex.getTheta() + angle
    );
    Cartesian rotatedVertex(
        rotatedRelativeVertex.getX() + point.getX(),
        rotatedRelativeVertex.getY() + point.getY()
    );
    return rotatedVertex;
}

Polygon GeometryUtilities::createCirclePolygon(const Cartesian& position, const Distance& radius, int numberOfEdges) {
    SIM_ASSERT_LE(3, numberOfEdges);
    std::vector<Cartesian> vertices;
    for (int i = 0; i < numberOfEdges; i += 1) {
        vertices.push_back(Polar(radius, Radians(i * M_TWOPI / numberOfEdges)) + position);
    }
    return Polygon(vertices);
}

MetersSquared GeometryUtilities::crossProduct(const Cartesian& Z, const Cartesian& A, const Cartesian& B) {

    // The cross product of ZA and ZB is simply the determinant of the following matrix:
    //
    //                                |A_x-Z_x, A_y-Z_y|
    //                                |B_x-Z_x, B_y-Z_y|
    //
    // Where Z is simply the location of the origin for the vectors A and B

    return (A.getX() - Z.getX()) * (B.getY() - Z.getY()) - (A.getY() - Z.getY()) * (B.getX() - Z.getX());
}

Polygon GeometryUtilities::convexHull(const std::vector<Polygon>& polygons) {

    // TODO: Explain/Clean this functionality

    // Implementation of Andrew's monotone chain 2D convex hull algorithm
    // Asymptotic complexity: O(n log n).
    // Returns a list of points on the convex hull in counter-clockwise order.
    // Note: the last point in the returned list is the same as the first one.

    // First, get a list of all of the points
    std::vector<Cartesian> points;
    for (Polygon polygon : polygons) {
        for (Cartesian point : polygon.getVertices()) {
            points.push_back(point);
        }
    }

    int n = points.size();
    int k = 0;
    std::vector<Cartesian> hull(2*n);

    // Sort points lexicographically
    std::sort(points.begin(), points.end());

    // Build lower hull
    for (int i = 0; i < n; i += 1) {
        while (k >= 2 && crossProduct(hull[k-2], hull[k-1], points[i]).getMetersSquared() <= 0) {
            k--;
        }
        hull[k++] = points[i];
    }

    // Build upper hull
    for (int i = n-2, t = k+1; i >= 0; i--) {
        while (k >= t && crossProduct(hull[k-2], hull[k-1], points[i]).getMetersSquared() <= 0) {
            k--;
        }
        hull[k++] = points[i];
    }

    hull.resize(k);

    return Polygon(hull);
}

Cartesian GeometryUtilities::castRay(
        const Cartesian& start, const Cartesian& end, const Maze& maze,
        const Meters& halfWallWidth, const Meters& tileLength) {

    // This is an implementation of ray-casting, a quick way to determine the
    // first object with which a ray collides. It relies on the fact that we
    // know where the walls are ahead of time.

    // First, determine the difference between the points. This allows us to
    // determine the direction of the ray, and thus the logical starting and
    // ending tiles (different from the actual starting and ending tiles).
    Meters dx = end.getX() - start.getX();
    Meters dy = end.getY() - start.getY();

    // Determine the direction of the ray
    std::pair<int, int> direction = std::make_pair(
        (0 < dx.getMeters() ? 1 : -1),
        (0 < dy.getMeters() ? 1 : -1)
    );

    //  Logical Tiles
    //  =============
    //  +-----------------+-----------------+-----------------+
    //  |:::|.........|:::|:::|.........|:::|:::|.........|:::|
    //  |---+---------+---|---+---------+---|---+---------+---|
    //  |...|         |...|...|         |...|...|         |...|
    //  |...|         |...|...|         |...|...|         |...|
    //  |...|(x-1,y+1)|...|...| (x,y+1) |...|...|(x+1,y+1)|...|
    //  |...|         |...|...|         |...|...|         |...|
    //  |---+--------[J]--|--[N]-------[K]--|--[O]--------+---|
    //  |:::|.........|:::|:::|.........|:::|:::|.........|:::|
    //  +----------------[B]---------------[C]----------------+
    //  |:::|.........|:::|:::|.........|:::|:::|.........|:::|
    //  |---+--------[F]--|--[R]-------[G]--|--[S]--------+---|
    //  |...|         |...|...|         |...|...|         |...|
    //  |...|         |...|...|         |...|...|         |...|
    //  |...| (x-1,y) |...|...|  (x,y)  |...|...| (x+1,y) |...|
    //  |...|         |...|...|         |...|...|         |...|
    //  |---+--------[I]--|--[M]-------[L]--|--[P]--------+---|
    //  |:::|.........|:::|:::|.........|:::|:::|.........|:::|
    //  +----------------[A]---------------[D]----------------+
    //  |:::|.........|:::|:::|.........|:::|:::|.........|:::|
    //  |---+--------[E]--|--[Q]-------[H]--|--[T]--------+---|
    //  |...|         |...|...|         |...|...|         |...|
    //  |...|         |...|...|         |...|...|         |...|
    //  |...|(x-1,y-1)|...|...| (x,y-1) |...|...|(x+1,y-1)|...|
    //  |...|         |...|...|         |...|...|         |...|
    //  |---+---------+---|---+---------+---|---+---------+---|
    //  |:::|.........|:::|:::|.........|:::|:::|.........|:::|
    //  +-----------------+-----------------+-----------------+
    //
    // Consider the groups of nine tiles drawn above. The potential wall
    // locations are filled with '.' characters, and the corner locations are
    // filled with ':' characters. The normal boundaries of the tile (x,y) are
    // given by ABCD. If we let these be the boundaries for the ray-casting,
    // the rays will terminate in the center of the walls, which is not what we
    // want. Instead, we want the rays to terminate at the boundaries of the
    // walls. The way that we achieve this is by calculating the next possible
    // ray termination location based on the direction of the ray. That is, if
    // the ray is traveling north east, collisions can only happen on the south
    // west walls. Thus, if we start by assuming that all possible collision
    // points are in the center of the tiles, then we can shift these collision
    // points/boundaries down and to the left (by one half wall width) to
    // determine where the ray would actually collide with the edge of the wall.
    // In this case, we'd want the "logical" tile boundaries to align with EFGH.
    // Similarly, if the ray was traveling north west, we want to shift the
    // collision boundaries to the south east, and we'd like the logical tile
    // boundaries to align with QRST. The same follows for south east and south
    // west rays, and IJKL and MNOP, respectively.

    // We want to shift the walls in the opposite direction of the ray
    Cartesian logicalShift = Cartesian(
        halfWallWidth * direction.first  * -1,
        halfWallWidth * direction.second * -1
    );

    // The current x and y positions that are tracked in the loop
    Meters cx = start.getX();
    Meters cy = start.getY();

    // Determine the logical starting tile
    int sx = static_cast<int>(std::floor((start - logicalShift).getX() / tileLength));
    int sy = static_cast<int>(std::floor((start - logicalShift).getY() / tileLength)); 

    // The initial integer tile offset from the starting tile
    int px = (direction.first  == 1 ? 1 : 0);
    int py = (direction.second == 1 ? 1 : 0);

    // The current integer tile offset from the starting tile
    int ox = px;
    int oy = py;

    // The increment to take on the offset values
    int ix = direction.first;
    int iy = direction.second;

    // The x and y values of the next potential collision
    Meters nx = tileLength * (sx + ox) + logicalShift.getX();
    Meters ny = tileLength * (sy + oy) + logicalShift.getY();

    // The direction of wall to inspect for a potential collision
    Direction wx = (direction.first  == 1 ? Direction::EAST  : Direction::WEST );
    Direction wy = (direction.second == 1 ? Direction::NORTH : Direction::SOUTH);

    // The x and y conditions on which to continue looping
    static std::function<bool(const Meters&, const Meters&)> east = [](const Meters& nx, const Meters& ex) {
        return nx < ex;
    };
    static std::function<bool(const Meters&, const Meters&)> west = [](const Meters& nx, const Meters& ex) {
        return ex < nx;
    };
    static std::function<bool(const Meters&, const Meters&)> north = [](const Meters& ny, const Meters& ey) {
        return ny < ey;
    };
    static std::function<bool(const Meters&, const Meters&)> south = [](const Meters& ny, const Meters& ey) {
        return ey < ny;
    };
    std::function<bool(const Meters&, const Meters&)>* bx = (direction.first  == 1 ? &east  : &west );
    std::function<bool(const Meters&, const Meters&)>* by = (direction.second == 1 ? &north : &south);

    // Loop until we've exhausted the entirety of the ray
    while ((*bx)(nx, end.getX()) || (*by)(ny, end.getY())) {

        // x collision will happen first
        if (std::abs((nx - cx) / dx) < std::abs((ny - cy) / dy)) {
            cy = cy + (nx - cx) * (dy / dx);
            cx = nx;
            int x = sx + ox - px;
            int y = sy + oy - py;
            if (isOnTileEdge(cy, halfWallWidth, tileLength) ||
                    (maze.withinMaze(x, y) && maze.getTile(x, y)->isWall(wx))) {
                return Cartesian(cx, cy);
            }
            ox += ix;
            nx = tileLength * (sx + ox) + logicalShift.getX();
        }

        // y collision will happen first
        else {
            cx = cx + (ny - cy) * (dx / dy);
            cy = ny;
            int x = sx + ox - px;
            int y = sy + oy - py;
            if (isOnTileEdge(cx, halfWallWidth, tileLength) ||
                    (maze.withinMaze(x, y) && maze.getTile(x, y)->isWall(wy))) {
                return Cartesian(cx, cy);
            }
            oy += iy;
            ny = tileLength * (sy + oy) + logicalShift.getY();
        }
    }

    return end;
}

bool GeometryUtilities::isOnTileEdge(
        const Meters& position, const Meters& halfWallWidth, const Meters& tileLength) {
    Meters tileLengthMinusHalfWallWidth = tileLength - halfWallWidth;
    Meters mod = Meters(std::fmod(position.getMeters(), tileLength.getMeters()));
    return (mod < halfWallWidth || tileLengthMinusHalfWallWidth < mod);
}

} // namespace sim

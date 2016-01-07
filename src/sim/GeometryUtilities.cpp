#include "GeometryUtilities.h"

#include <algorithm>

#include "Assert.h"
#include "CPMath.h"
#include "Param.h" // TODO: MACK: KILL THIS
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
    ASSERT_LE(3, numberOfEdges);
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

bool GeometryUtilities::linesIntersect(const std::pair<const Cartesian&, const Cartesian&>& A,
                    const std::pair<const Cartesian&, const Cartesian&>& B) {

    // Two line segments intersect if the points of the each of the segments are not
    // on the same side of the line connecting the points of the other segment:
    //
    //                                     B2
    //                                      |
    //                                  A1--|--A2
    //                                      |
    //                                     B1
    //
    // In this case, A1 and A2 are not on the same side of the line connecting B1 to B2,
    // and B1 and B2 are not on the same side of the line connecting A1 and A2. Thus, the
    // line segments intersect.
    //
    // A relatively easy way to check to see that A1 and A2 are on opposite sides of the
    // line connecting B1 and B2 is to compare the cross product of A1 and B2 with
    // the cross product of A2 and B2 (both relative to B1). If the cross products have
    // opposite signs, this means that the we have to travel in a different direction
    // (clockwise or counter-clockwise) to connect A1 to B2 than we do to have to connect
    // A2 to B2 (by the right-hand rule for cross products). Thus A1 and A2 are on opposite
    // sides of the line connecting B1 to B2. For example:
    //
    //         B2
    //          |
    //      A1  |  (relative to B1, we travel clockwise to get from A1 to B2)
    //        \ |
    //         B1
    //
    //         B2
    //          |
    //          |  A2  (relative to B1, we travel counter-clockwise to get from A2 to B2)
    //          | /
    //         B1
    //
    // We can do the same test to see that B1 and B2 are on opposite sides of the segment
    // connecting A1 and A2.

    // Check to see that the points of B are on opposite sides of A
    double c1 = crossProduct(A.first, B.first,  A.second).getMetersSquared();
    double c2 = crossProduct(A.first, B.second, A.second).getMetersSquared();

    // Check to see that the points of A are on opposite sides of B
    double c3 = crossProduct(B.first, A.first,  B.second).getMetersSquared();
    double c4 = crossProduct(B.first, A.second, B.second).getMetersSquared();

    // Lastly, a cheap way to check to see that the cross products have opposite signs
    // (or that one is zero) is to multiply them together and check to see the product
    // is less than or equal to zero.

    return (c1*c2 <= 0 && c3*c4 <= 0);
}

Cartesian GeometryUtilities::getIntersectionPoint(const std::pair<const Cartesian&, const Cartesian&>& A,
                                                  const std::pair<const Cartesian&, const Cartesian&>& B) {

    // TODO: Explain/Clean this functionality

    // Assert that the lines do intersect
    ASSERT_TR(linesIntersect(A, B));

    // Taken from http://alienryderflex.com/intersect/

    // Extract A's points
    double a1x = A.first.getX().getMeters();
    double a1y = A.first.getY().getMeters();
    double a2x = A.second.getX().getMeters();
    double a2y = A.second.getY().getMeters();

    // Extract B's points
    double b1x = B.first.getX().getMeters();
    double b1y = B.first.getY().getMeters();
    double b2x = B.second.getX().getMeters();
    double b2y = B.second.getY().getMeters();

    // Translate the system so that point A is on the origin.
    a2x -= a1x;
    a2y -= a1y;
    b1x -= a1x;
    b1y -= a1y;
    b2x -= a1x;
    b2y -= a1y;

    // Discover the length of segment A-B.
    double distAB = sqrt(a2x*a2x+a2y*a2y);

    // Rotate the system so that point B is on the positive X axis.
    double theCos = a2x / distAB;
    double theSin = a2y / distAB;
    double newX = b1x * theCos + b1y * theSin;
    b1y = b1y * theCos - b1x * theSin;
    b1x = newX;
    newX = b2x * theCos + b2y * theSin;
    b2y = b2y * theCos - b2x * theSin;
    b2x = newX;

    // Discover the position of the intersection point along line A-B.
    double ABpos = b2x + (b1x - b2x) * b2y / (b2y - b1y);

    // Apply the discovered position to line A-B in the original coordinate system.
    return Cartesian(Meters(a1x+ABpos*theCos), Meters(a1y+ABpos*theSin));
}

std::vector<const Tile*> GeometryUtilities::lineSegmentTileCover(const Cartesian& A, const Cartesian& B, const Maze& maze) {

    // The output vector
    std::vector<const Tile*> tilesInRange;

    // The length of any one tile
    Meters tileLength = P()->wallLength() + P()->wallWidth();

    // Lexicographical ordering
    Cartesian p0 = std::min(A, B);
    Cartesian p1 = std::max(A, B);

    // Starting tile
    int x0 = static_cast<int>(std::floor(p0.getX() / tileLength));
    int y0 = static_cast<int>(std::floor(p0.getY() / tileLength)); 

    // Ending tile
    int x1 = static_cast<int>(std::floor(p1.getX() / tileLength));
    int y1 = static_cast<int>(std::floor(p1.getY() / tileLength));

    // Difference between the points
    Meters dx = p1.getX() - p0.getX(); // Always non-negative
    Meters dy = p1.getY() - p0.getY(); // Could have any sign

    // For all y-intercepts to the right of the the starting point
    for (int i = 1; tileLength * (x0 + i) < p1.getX(); i += 1) {
        Meters fx = tileLength * (x0 + i);
        Meters fy = p0.getY() + (fx - p0.getX()) * (dy / dx);
        // Add the tile that is to the left of the y-intercept 
        int x = x0 + i - 1;
        int y = static_cast<int>(std::floor(fy / tileLength));
        if (0 <= x && x < maze.getWidth() && 0 <= y && y < maze.getHeight()) {
            tilesInRange.push_back(maze.getTile(x, y));
        }
    }

    // For all x-intercepts above the the starting point, if the slope is positive ...
    if (0 < dy.getMeters()) {
        for (int i = 1; tileLength * (y0 + i) < p1.getY(); i += 1) {
            Meters fy = tileLength * (y0 + i);
            Meters fx = p0.getX() + (fy - p0.getY()) * (dx / dy);
            // Add the tile that is below the x-intercept
            int x = static_cast<int>(std::floor(fx / tileLength));
            int y = y0 + i - 1;
            if (0 <= x && x < maze.getWidth() && 0 <= y && y < maze.getHeight()) {
                tilesInRange.push_back(maze.getTile(x, y));
            }
        }
    }

    // ... or for all x-intercepts below the the starting point, if the slope is negative
    else {
        for (int i = 0; p1.getY() < tileLength * (y0 - i); i += 1) {
            Meters fy = tileLength * (y0 - i);
            Meters fx = p0.getX() + (fy - p0.getY()) * (dx / dy);
            // Add the tile that is above the x-intercept
            int x = static_cast<int>(std::floor(fx / tileLength));
            int y = y0 - i;
            if (0 <= x && x < maze.getWidth() && 0 <= y && y < maze.getHeight()) {
                tilesInRange.push_back(maze.getTile(x, y));
            }
        }
    }

    // Lastly, add the destination tile
    if (0 <= x1 && x1 < maze.getWidth() && 0 <= y1 && y1 < maze.getHeight()) {
        tilesInRange.push_back(maze.getTile(x1, y1));
    }

    // Sort the tiles lexicographically
    std::sort(tilesInRange.begin(), tilesInRange.end());
    return tilesInRange;
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
        if ((nx - cx) / dx < (ny - cy) / dy) {
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

#include "GeometryUtilities.h"

#include <algorithm>
#include <cmath>

#include "Assert.h"
#include "Param.h"

namespace sim {

std::vector<const Tile*> GeometryUtilities::lineSegmentTileCover(const Cartesian& A, const Cartesian& B, const Maze& maze) {

    // The output vector
    std::vector<const Tile*> tilesInRange;

    // The length of any one tile
    Meters tileLength = P()->wallLength() + P()->wallWidth();

    // Lexicographical ordering
    Cartesian p0 = std::min(A, B);
    Cartesian p1 = std::max(A, B);

    // Starting tile
    int x0 = static_cast<int>(floor(p0.getX() / tileLength));
    int y0 = static_cast<int>(floor(p0.getY() / tileLength)); 

    // Ending tile
    int x1 = static_cast<int>(floor(p1.getX() / tileLength));
    int y1 = static_cast<int>(floor(p1.getY() / tileLength));

    // Difference between the points
    Meters dx = p1.getX() - p0.getX(); // Always non-negative
    Meters dy = p1.getY() - p0.getY(); // Could have any sign

    // For all y-intercepts to the right of the the starting point
    for (int i = 1; tileLength * (x0 + i) < p1.getX(); i += 1) {
        Meters fx = tileLength * (x0 + i);
        Meters fy = p0.getY() + (fx - p0.getX()) * (dy / dx);
        // Add the tile that is to the left of the y-intercept 
        int x = x0 + i - 1;
        int y = static_cast<int>(floor(fy / tileLength));
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
            int x = static_cast<int>(floor(fx / tileLength));
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
            int x = static_cast<int>(floor(fx / tileLength));
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
    float c1 = crossProduct(A.first, B.first,  A.second).getMetersSquared();
    float c2 = crossProduct(A.first, B.second, A.second).getMetersSquared();

    // Check to see that the points of A are on opposite sides of B
    float c3 = crossProduct(B.first, A.first,  B.second).getMetersSquared();
    float c4 = crossProduct(B.first, A.second, B.second).getMetersSquared();

    // Lastly, a cheap way to check to see that the cross products have opposite signs
    // (or that one is zero) is to multiply them together and check to see the product
    // is less than or equal to zero.

    return (c1*c2 <= 0 && c3*c4 <= 0);
}

Cartesian GeometryUtilities::getIntersectionPoint(const std::pair<const Cartesian&, const Cartesian&>& A,
                                                  const std::pair<const Cartesian&, const Cartesian&>& B) {

    // TODO: SOM - Explain/Clean this functionality - right now, I don't know how it works

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

MetersSquared GeometryUtilities::polygonArea(const Polygon& polygon) {

    // Magic (not really - see http://mathworld.wolfram.com/PolygonArea.html)
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

std::vector<std::pair<Cartesian, Cartesian>> GeometryUtilities::getLineSegments(const Polygon& polygon) {

    std::vector<std::pair<Cartesian, Cartesian>> segments;

    std::vector<Cartesian> vertices = polygon.getVertices();
    if (1 < vertices.size()) {
        Cartesian previousPoint = vertices.back();
        for (Cartesian currentPoint : vertices) {
            segments.push_back(std::make_pair(previousPoint, currentPoint));
            previousPoint = currentPoint;
        }
    }

    return segments;
}

Polygon GeometryUtilities::convexHull(const std::vector<Polygon>& polygons) {

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

Polygon GeometryUtilities::getUnionMultiple(const std::vector<Polygon>& polygons) {
    // TODO: SOM - This function should return the union of the polygons
    // Assumptions:
    // - The polygons are connected
    // - The output polygon has no holes in it
    // - None of the polygons are self-intersecting
}

Polygon GeometryUtilities::getUnionTwo(const Polygon& A, const Polygon& B) {
    std::vector<Cartesian> APoints = A.getVertices();
    std::vector<Cartesian> BPoints = B.getVertices();

    auto APoint = APoints.begin();

    std::vector<std::pair<Cartesian, Cartesian>> AEdges = getLineSegments(A);
    std::vector<std::pair<Cartesian, Cartesian>> BEdges = getLineSegments(B);

    //For each edge in polygon A
    for(std::pair<Cartesian, Cartesian> edgeA : AEdges) {
        auto BPoint = BPoints.begin();
        //For each edge in Polygon B
        for(std::pair<Cartesian, Cartesian> edgeB : BEdges) {
            std::vector<Cartesian> intersects;
            if(linesIntersect(edgeA, edgeB)) {
                Cartesian intersect = getIntersectionPoint(edgeA, edgeB);
                intersects.push_back(intersect);
                //Insert intersect into BPoints
            }
        }
        //Sort points in intersects by distance from start point
        //Insert intersects into APoints 
    }

    return A;
}

MetersSquared GeometryUtilities::crossProduct(const Cartesian& Z, const Cartesian& A, const Cartesian& B) {

    // The cross product of ZA and ZB is simply the determinant of the following matrix:
    //
    //                                |AX-ZX, AY-ZY|
    //                                |BX-ZX, BY-ZY|
    //
    // Where Z is simply the location of the origin for the vectors A and B

    return (A.getX() - Z.getX()) * (B.getY() - Z.getY()) - (A.getY() - Z.getY()) * (B.getX() - Z.getX());
}

} // namespace sim

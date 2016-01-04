#include "Sensor.h"

#include <algorithm>

#include "units/Polar.h"

#include "CPMath.h"
#include "GeometryUtilities.h"
#include "Param.h"

#include "Logging.h" // TODO: MACK

namespace sim {

Sensor::Sensor(
        const Distance& radius,
        const Distance& range,
        const Angle& halfWidth,
        const Duration& readDuration,
        const Coordinate& position,
        const Angle& direction) :
        m_range(range),
        m_halfWidth(halfWidth),
        m_readDuration(readDuration),
        m_initialPosition(position),
        m_initialDirection(direction) {

    // Create the polygon for the body of the sensor
    m_initialPolygon = GeometryUtilities::createCirclePolygon(position, radius, P()->numberOfCircleApproximationPoints());

    // Create the polygon for the view of the sensor
    std::vector<Cartesian> view;
    view.push_back(position);
    for (double i = -1; i <= 1; i += 2.0/(P()->numberOfSensorEdgePoints() - 1)) {
        view.push_back(Polar(range, (Radians(halfWidth) * i) + direction) + position);
    }

    m_initialViewPolygon = Polygon(view);
    // TODO: MACK - update the current polygon here
    m_currentViewPolygon = m_initialViewPolygon;
}

Seconds Sensor::getReadDuration() const {
    return m_readDuration;
}

Cartesian Sensor::getInitialPosition() const {
    return m_initialPosition;
}

Radians Sensor::getInitialDirection() const {
    return m_initialDirection;
}

Polygon Sensor::getInitialPolygon() const {
    return m_initialPolygon;
}

Polygon Sensor::getInitialViewPolygon() const {
    return m_initialViewPolygon;
}

Polygon Sensor::getCurrentViewPolygon() const {
    return m_currentViewPolygon;
}

void Sensor::updateCurrentViewPolygon(
        const Cartesian& currentPosition,
        const Radians& currentDirection,
        const Maze& maze) {

    // TODO: MACK
    updateCurrentViewPolygon2(currentPosition, currentDirection, maze);
    return;

    // First, get the edge of the view of the sensor
    std::vector<Cartesian> edge;
    for (double i = -1; i <= 1; i += 2.0/ (P()->numberOfSensorEdgePoints() - 1)) {
        edge.push_back(currentPosition + Polar(m_range, currentDirection + (m_halfWidth * i)));
    }

    // For each point along the edge of the view ...
    for (int i = 0; i < edge.size(); i += 1) {

        // ... for each tile within the range of the edge point ...
        for (const Tile* tile : GeometryUtilities::lineSegmentTileCover(currentPosition, edge.at(i), maze)) {

            // ... iterate through all of the tile's polygons ...
            for (const std::vector<Polygon>& group : {tile->getActualWallPolygons(), tile->getCornerPolygons()}) {
                for (const Polygon& obstacle : group) {
                    for (const std::pair<Cartesian, Cartesian>& A : obstacle.getLineSegments()) {

                        // ... and check for intersections
                        std::pair<Cartesian, Cartesian> B = std::make_pair(currentPosition, edge.at(i));
                        if (GeometryUtilities::linesIntersect(A, B)) {
                            edge.at(i) = GeometryUtilities::getIntersectionPoint(A, B);
                        }
                    }
                }
            }
        }
    }

    // Adjoin the edge to the currentPosition and return the polygon
    edge.insert(edge.begin(), currentPosition);

    // Finally, update the current view polygon
    m_currentViewPolygon = Polygon(edge);
}

void Sensor::updateCurrentViewPolygon2(
        const Cartesian& currentPosition,
        const Radians& currentDirection,
        const Maze& maze) {

    // TODO: MACK - improve this

    // First, get the edge of the view of the sensor
    std::vector<Cartesian> edge;
    for (double i = -1; i <= 1; i += 2.0/ (P()->numberOfSensorEdgePoints() - 1)) {
        edge.push_back(currentPosition + Polar(m_range, currentDirection + (m_halfWidth * i)));
    }

    // For each point along the edge of the view ...
    for (int i = 0; i < edge.size(); i += 1) {
        edge.at(i) = getEnd(currentPosition, edge.at(i), maze);

        /*
        // ... for each tile within the range of the edge point ...
        for (const Tile* tile : GeometryUtilities::lineSegmentTileCover(currentPosition, edge.at(i), maze)) {

            // ... iterate through all of the tile's polygons ...
            for (const std::vector<Polygon>& group : {tile->getActualWallPolygons(), tile->getCornerPolygons()}) {
                for (const Polygon& obstacle : group) {
                    for (const std::pair<Cartesian, Cartesian>& A : obstacle.getLineSegments()) {

                        // ... and check for intersections
                        std::pair<Cartesian, Cartesian> B = std::make_pair(currentPosition, edge.at(i));
                        if (GeometryUtilities::linesIntersect(A, B)) {
                            edge.at(i) = GeometryUtilities::getIntersectionPoint(A, B);
                        }
                    }
                }
            }
        }
        */
    }

    // Adjoin the edge to the currentPosition and return the polygon
    edge.insert(edge.begin(), currentPosition);

    // Finally, update the current view polygon
    m_currentViewPolygon = Polygon(edge);
}

Cartesian Sensor::getEnd(Cartesian A, Cartesian B, const Maze& maze) {

    // This is an implementation of ray-casting, a quick way to determine the
    // first object with which a ray collides. It relies on the fact that we
    // know where the walls are ahead of time.

    // Some often used lengths
    static Meters halfWallWidth = Meters(P()->wallWidth() / 2.0);
    static Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    static Meters halfWallWidthFromTileLength = tileLength - halfWallWidth;

    // Determine the starting tile
    int sx = static_cast<int>(std::floor(A.getX() / tileLength));
    int sy = static_cast<int>(std::floor(A.getY() / tileLength)); 

    // The initial integer tile offset from the starting tile
    int px = 0;
    int py = 0;

    // The current integer tile offset from the starting tile
    int ox = 0;
    int oy = 0;

    // The increment to take on the offset values
    int ix = 0;
    int iy = 0;

    // Difference between the points
    Meters dx = B.getX() - A.getX();
    Meters dy = B.getY() - A.getY();

    // The current x and y positions that are tracked in the loop
    Meters cx = A.getX();
    Meters cy = A.getY();

    // The x and y values of the next potential collision
    // TODO: MACK - move these down
    Meters nx = Meters(0);
    Meters ny = Meters(0);

    // TODO: MACK - get rid of this
    // The direction of wall to inspect
    Direction wx;
    Direction wy;

    // TODO: MACK - clean this up, take values as args if possible
    // The condition on which to continue looping
    static std::function<bool()> east = [&]() {
        return nx < B.getX();
    };
    static std::function<bool()> west = [&]() {
        return B.getX() < nx;
    };
    static std::function<bool()> north = [&]() {
        return ny < B.getY();
    };
    static std::function<bool()> south = [&]() {
        return B.getY() < ny;
    };

    // The x and y conditions for the loop
    std::function<bool()>* bx;
    std::function<bool()>* by;

    // TODO: MACK - don't use fmod
    // The current position, shifted to the beginning tile
    /*
    Meters xMod = Meters(std::fmod(A.getX().getMeters(), tileLength.getMeters()));
    Meters yMod = Meters(std::fmod(A.getY().getMeters(), tileLength.getMeters()));
    bool xInWall = (xMod < halfWallWidth || halfWallWidthFromTileLength < xMod);
    bool yInWall = (yMod < halfWallWidth || halfWallWidthFromTileLength < yMod);
    */
    /*
    L()->info("(%v,%v)", halfWallWidth.getMeters(), (tileLength - halfWallWidth).getMeters());
    L()->info("(%v,%v)", xMod.getMeters(), yMod.getMeters());
    L()->info("(%v,%v)", (xInWall ? "true" : "false"), (yInWall ? "true" : "false"));
    return Cartesian(Meters(0), Meters(0));
    */

    // TODO: MACK
    /*
    if (dx.getMeters() == 0) {
        return Cartesian(Meters(0), Meters(0));
    }
    if (dy.getMeters() == 0) {
        return Cartesian(Meters(0), Meters(0));
    }
    */

    // If the ray is traveling east
    if (0 < dx.getMeters()) {
        ix = 1;
        wx = Direction::EAST;
        bx = &east;
    }

    // If the ray is traveling west
    else {
        ix = -1; // TODO: MACK -1 is too far ... it should just be the current tile, right?
        wx = Direction::WEST;
        bx = &west;
    }

    // If the ray is traveling north
    if (0 < dy.getMeters()) {
        iy = 1;
        wy = Direction::NORTH;
        by = &north;
    }

    // If the ray is traveling south
    else {
        iy = -1;
        wy = Direction::SOUTH;
        by = &south;
    }

    // Set the discrete offset, i.e., identify the next tile to inspect
    // TODO: MACK - the declaration should be here
    px = (ix == 1 ? 1 : 0);
    py = (iy == 1 ? 1 : 0);
    ox = px;
    oy = py;

    // The x and y values of the next potential collision
    nx = tileLength * (sx + ox);
    ny = tileLength * (sy + oy);

    // Loop until we've exhausted the entirety of the ray
    while ((*bx)() || (*by)()) {

        // x collision will happen first
        if ((nx - cx) / dx < (ny - cy) / dy) {
            cy = cy + (nx - cx) * (dy / dx);
            cx = nx;
            if (maze.getTile(sx + ox - px, sy + oy - py)->isWall(wx)) {
                return Cartesian(cx, cy);
            }
            ox += ix;
            nx = tileLength * (sx + ox);
        }

        // y collision will happen first
        else {
            cx = cx + (ny - cy) * (dx / dy);
            cy = ny;
            if (maze.getTile(sx + ox - px, sy + oy - py)->isWall(wy)) {
                return Cartesian(cx, cy);
            }
            oy += iy;
            ny = tileLength * (sy + oy);
        }
    }


    /*
    // If the ray is traveling east
    if (0 < dx.getMeters()) {

        // If the ray is traveling north
        if (0 < dy.getMeters()) {

            // TODO: MACK
            ix = 1;
            iy = 1;
            wx = Direction::EAST;
            wy = Direction::NORTH;
            bx = &east;
            by = &north;

            // Set the discrete offset, i.e., identify the next tile to inspect
            ox += ix;
            oy += ix;

            // The x and y values of the next potential collision
            nx = tileLength * (sx + ox);
            ny = tileLength * (sy + oy);

            // Loop until we've exhausted the entirety of the ray
            while ((*bx)() || (*by)()) {

                // x collision will happen first
                if ((nx - cx) / dx < (ny - cy) / dy) {
                    cy = cy + (nx - cx) * (dy / dx);
                    cx = nx;
                    if (maze.getTile(sx + ox - ix, sy + oy - iy)->isWall(wx)) {
                        return Cartesian(cx, cy);
                    }
                    ox += ix;
                    nx = tileLength * (sx + ox);
                }

                // y collision will happen first
                else {
                    cx = cx + (ny - cy) * (dx / dy);
                    cy = ny;
                    if (maze.getTile(sx + ox - ix, sy + oy - iy)->isWall(wy)) {
                        return Cartesian(cx, cy);
                    }
                    oy += iy;
                    ny = tileLength * (sy + oy);
                }
            }
        }

        // Else the ray is traveling south
        else {
        }
    }

    // Else the ray is traveling west
    else {

        // If the ray is traveling north
        if (0 < dy.getMeters()) {
        }

        // Else the ray is traveling south
        else {
        }
    }
    */

    /*
    // For all y-intercepts to the right of the the starting point
    for (int i = 1; tileLength * (x0 + i) < B.getX(); i += 1) {
        Meters fx = tileLength * (x0 + i);
        Meters fy = A.getY() + (fx - A.getX()) * (dy / dx);
        // Add the tile that is to the right of the y-intercept 
        int x = x0 + i - 1;
        int y = static_cast<int>(std::floor(fy / tileLength));
        if (0 <= x && x < maze.getWidth() && 0 <= y && y < maze.getHeight()) {
            if (maze.getTile(x, y)->isWall(Direction::WEST)) { // TODO: MACK
                return Cartesian(Meters(0), Meters(0));  
            }
            //tilesInRange.push_back(maze.getTile(x, y));
        }
    }

    // For all x-intercepts above the the starting point, if the slope is positive ...
    if (0 < dy.getMeters()) {
        for (int i = 1; tileLength * (y0 + i) < B.getY(); i += 1) {
            Meters fy = tileLength * (y0 + i);
            Meters fx = A.getX() + (fy - A.getY()) * (dx / dy);
            // Add the tile that is below the x-intercept
            int x = static_cast<int>(std::floor(fx / tileLength));
            int y = y0 + i - 1;
            if (0 <= x && x < maze.getWidth() && 0 <= y && y < maze.getHeight()) {
                //tilesInRange.push_back(maze.getTile(x, y));
            }
        }
    }

    // ... or for all x-intercepts below the the starting point, if the slope is negative
    else {
        for (int i = 0; B.getY() < tileLength * (y0 - i); i += 1) {
            Meters fy = tileLength * (y0 - i);
            Meters fx = A.getX() + (fy - A.getY()) * (dx / dy);
            // Add the tile that is above the x-intercept
            int x = static_cast<int>(std::floor(fx / tileLength));
            int y = y0 - i;
            if (0 <= x && x < maze.getWidth() && 0 <= y && y < maze.getHeight()) {
                //tilesInRange.push_back(maze.getTile(x, y));
            }
        }
    }

    // Lastly, add the destination tile
    if (0 <= x1 && x1 < maze.getWidth() && 0 <= y1 && y1 < maze.getHeight()) {
        //tilesInRange.push_back(maze.getTile(x1, y1));
    }

    // Sort the tiles lexicographically
    //std::sort(tilesInRange.begin(), tilesInRange.end());
    */

    return B; // TODO: MACK
}

} // namespace sim

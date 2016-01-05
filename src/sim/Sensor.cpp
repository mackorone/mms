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
    }

    // Adjoin the edge to the currentPosition and return the polygon
    edge.insert(edge.begin(), currentPosition);

    // Finally, update the current view polygon
    m_currentViewPolygon = Polygon(edge);
}

Cartesian Sensor::getEnd(Cartesian start, Cartesian end, const Maze& maze) {

    // This is an implementation of ray-casting, a quick way to determine the
    // first object with which a ray collides. It relies on the fact that we
    // know where the walls are ahead of time.

    // Some often used lengths
    static Meters halfWallWidth = Meters(P()->wallWidth() / 2.0);
    static Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());

    // Determine the starting tile
    int sx = static_cast<int>(std::floor(start.getX() / tileLength));
    int sy = static_cast<int>(std::floor(start.getY() / tileLength)); 

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
    Meters dx = end.getX() - start.getX();
    Meters dy = end.getY() - start.getY();

    // The current x and y positions that are tracked in the loop
    Meters cx = start.getX();
    Meters cy = start.getY();

    // The x and y values of the next potential collision
    Meters nx = Meters(0);
    Meters ny = Meters(0);

    // The direction of wall to inspect
    Direction wx;
    Direction wy;

    // The condition on which to continue looping
    static std::function<bool()> east = [&]() {
        return nx < end.getX();
    };
    static std::function<bool()> west = [&]() {
        return end.getX() < nx;
    };
    static std::function<bool()> north = [&]() {
        return ny < end.getY();
    };
    static std::function<bool()> south = [&]() {
        return end.getY() < ny;
    };
    std::function<bool()>* bx;
    std::function<bool()>* by;

    // If the ray is traveling east
    if (0 < dx.getMeters()) {
        ix = 1;
        px = 1;
        ox = 1;
        wx = Direction::EAST;
        bx = &east;
    }

    // If the ray is traveling west
    else {
        ix = -1;
        px = 0;
        ox = 0;
        wx = Direction::WEST;
        bx = &west;
    }

    // If the ray is traveling north
    if (0 < dy.getMeters()) {
        iy = 1;
        py = 1;
        oy = 1;
        wy = Direction::NORTH;
        by = &north;
    }

    // If the ray is traveling south
    else {
        iy = -1;
        py = 0;
        oy = 0;
        wy = Direction::SOUTH;
        by = &south;
    }

    // TODO: MACK - the problem is that the next x might not be beyond the
    // current x if it's in the tile's edge

    // The x and y values of the next potential collision
    /*
    if (isTileEdge(cx)) {
        ox += ix;
    }
    if (isTileEdge(cy)) {
        oy += iy;
    }
    nx = tileLength * (sx + ox) + halfWallWidth * (wx == Direction::EAST ? -1 : 1);
    ny = tileLength * (sy + oy) + halfWallWidth * (wy == Direction::NORTH ? -1 : 1);
    */
    nx = tileLength * (sx + ox);
    ny = tileLength * (sy + oy);

    // Loop until we've exhausted the entirety of the ray
    while ((*bx)() || (*by)()) {

        // x collision will happen first
        if ((nx - cx) / dx < (ny - cy) / dy) {
            cy = cy + (nx - cx) * (dy / dx);
            cx = nx;
            if (isTileEdge(cy) || maze.getTile(sx + ox - px, sy + oy - py)->isWall(wx)) {
                return Cartesian(cx, cy);
            }
            ox += ix;
            nx = tileLength * (sx + ox);
        }

        // y collision will happen first
        else {
            cx = cx + (ny - cy) * (dx / dy);
            cy = ny;
            if (isTileEdge(cx) || maze.getTile(sx + ox - px, sy + oy - py)->isWall(wy)) {
                return Cartesian(cx, cy);
            }
            oy += iy;
            ny = tileLength * (sy + oy);
        }
    }

    return end;
}

bool Sensor::isTileEdge(const Meters& pos) {
    static Meters halfWallWidth = Meters(P()->wallWidth() / 2.0);
    static Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    Meters mod = Meters(std::fmod(pos.getMeters(), tileLength.getMeters()));
    return (mod < halfWallWidth || tileLength - halfWallWidth < mod);
}

} // namespace sim

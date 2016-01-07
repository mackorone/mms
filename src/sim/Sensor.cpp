#include "Sensor.h"

#include <algorithm>

#include "units/Polar.h"

#include "CPMath.h"
#include "GeometryUtilities.h"
#include "Param.h"

namespace sim {

Sensor::Sensor(
        const Distance& radius,
        const Distance& range,
        const Angle& halfWidth,
        const Duration& readDuration,
        const Coordinate& position,
        const Angle& direction,
        const Maze& maze) :
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
    for (double i = -1; i <= 1; i += 2.0 / (P()->numberOfSensorEdgePoints() - 1)) {
        view.push_back(Polar(range, (Radians(halfWidth) * i) + direction) + position);
    }
    m_initialViewPolygon = Polygon(view);

    // Initialize the sensor reading
    updateReading(m_initialPosition, m_initialDirection, maze);
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

Polygon Sensor::getCurrentViewPolygon(
        const Cartesian& currentPosition,
        const Radians& currentDirection,
        const Maze& maze) const {
    return getViewPolygon(currentPosition, currentDirection, maze);
}

double Sensor::read() const {
    return m_currentReading;
}

void Sensor::updateReading(
        const Cartesian& currentPosition,
        const Radians& currentDirection,
        const Maze& maze) {

    m_currentReading = std::max(
        0.0,
        1.0 -
            getViewPolygon(currentPosition, currentDirection, maze).area() /
            getInitialViewPolygon().area());

    ASSERT_LE(0.0, m_currentReading);
    ASSERT_LE(m_currentReading, 1.0);
}

Polygon Sensor::getViewPolygon(
        const Cartesian& currentPosition,
        const Radians& currentDirection,
        const Maze& maze) const {

    std::vector<Cartesian> polygon {currentPosition};

    for (double i = -1; i <= 1; i += 2.0 / (P()->numberOfSensorEdgePoints() - 1)) {
        polygon.push_back(
            castRay(
                currentPosition,
                currentPosition + Polar(m_range, currentDirection + (m_halfWidth * i)),
                maze
            )
        );
    }

    return Polygon(polygon);
}

Cartesian Sensor::castRay(const Cartesian& start, const Cartesian& end, const Maze& maze) {

    // This is an implementation of ray-casting, a quick way to determine the
    // first object with which a ray collides. It relies on the fact that we
    // know where the walls are ahead of time.

    // Declare some often used lengths as static so as to only create them once.
    static Meters halfWallWidth = Meters(P()->wallWidth() / 2.0);
    static Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());

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
            if (isTileEdge(cy) || (maze.withinMaze(x, y) && maze.getTile(x, y)->isWall(wx))) {
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
            if (isTileEdge(cx) || (maze.withinMaze(x, y) && maze.getTile(x, y)->isWall(wy))) {
                return Cartesian(cx, cy);
            }
            oy += iy;
            ny = tileLength * (sy + oy) + logicalShift.getY();
        }
    }

    return end;
}

bool Sensor::isTileEdge(const Meters& pos) {
    static Meters halfWallWidth = Meters(P()->wallWidth() / 2.0);
    static Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    static Meters tileLengthMinusHalfWallWidth = tileLength - halfWallWidth;
    Meters mod = Meters(std::fmod(pos.getMeters(), tileLength.getMeters()));
    return (mod < halfWallWidth || tileLengthMinusHalfWallWidth < mod);
}

} // namespace sim

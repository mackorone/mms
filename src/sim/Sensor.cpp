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

    // First, get the edge of the view of the sensor
    std::vector<Cartesian> edge;
    for (double i = -1; i <= 1; i += 2.0/(P()->numberOfSensorEdgePoints() - 1)) {
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
    return Polygon(edge);
}

} // namespace sim

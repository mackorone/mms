#include "Sensor.h"

#include <algorithm>
#include <cmath>

#include <Polar.h>

#include "GeometryUtilities.h"
#include "Param.h"

namespace sim {

Sensor::Sensor(const Coordinate& position, const Distance& radius, const Angle& rotation, const Distance& range,
        const Angle& halfWidth, const Duration& readDuration) :
        m_initialTranslation(position), m_initialRotation(rotation), m_range(range), m_halfWidth(halfWidth),
        m_readDuration(readDuration) {

    // Create the polygon for the body of the sensor
    std::vector<Cartesian> polygon;
    for (int i = 1; i <= P()->numberOfCircleApproximationPoints(); i += 1) {
        float radians = i*2*M_PI/P()->numberOfCircleApproximationPoints();
        polygon.push_back(Polar(radius, Radians(radians)) + position);
    }
    m_initialPolygon = Polygon(polygon);

    // Create the polygon for the view of the sensor
    std::vector<Cartesian> view;
    view.push_back(position); // This needs to be first - the Mouse.cpp relies on this
    for (int i = -1*(P()->numberOfCircleApproximationPoints()/2); i <= P()->numberOfCircleApproximationPoints()/2; i += 1) {
        float radians = i*2*halfWidth.getRadians()/P()->numberOfCircleApproximationPoints();
        view.push_back(Polar(range, Radians(radians) + rotation) + position);
    }
    m_initialView = Polygon(view);
}

Cartesian Sensor::getInitialTranslation() const {
    return m_initialTranslation;
}

Radians Sensor::getInitialRotation() const {
    return m_initialRotation;
}

Polygon Sensor::getInitialPolygon() const {
    return m_initialPolygon;
}

Polygon Sensor::getInitialView() const {
    return m_initialView;
}

Polygon Sensor::getCurrentView(const Cartesian& currentPosition, const Radians& currentRotation, const Maze& maze) const {

    // TODO: If a vector is north-east, we only need to check the south-west walls, etc

    // First, get the edge of the view of the sensor
    std::vector<Cartesian> edge;
    for (float i = -1; i <= 1; i += 2.0/(P()->numberOfSensorEdgePoints() - 1)) {
        edge.push_back(currentPosition + Polar(m_range, currentRotation + (m_halfWidth * i)));
    }

    // For each point along the edge of the view ...
    for (int i = 0; i < edge.size(); i += 1) {

        // ... for each tile within the range of the edge point ...
        for (const Tile* tile : GeometryUtilities::lineSegmentTileCover(currentPosition, edge.at(i), maze)) {

            // ... iterate through all of the tile's polygons ...
            for (std::vector<Polygon> group : {tile->getActualWallPolygons(), tile->getCornerPolygons()}) {
                for (Polygon obstacle : group) {
                    for (std::pair<Cartesian, Cartesian> A : GeometryUtilities::getLineSegments(obstacle)) {

                        // TODO: Is there a way to "join" some of the line segments, so that we can do less work?

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

Seconds Sensor::getReadDuration() const {
    return m_readDuration;
}

} // namespace sim

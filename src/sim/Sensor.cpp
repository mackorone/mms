#include "Sensor.h"

#include <algorithm>
#include <cmath>
#include <Polar.h>

#include "GeometryUtilities.h"
#include "Param.h"

namespace sim {

Sensor::Sensor(const Coordinate& position, const Distance& radius, const Angle& rotation, const Distance& range,
        const Angle& halfWidth) :
        m_initialTranslation(position), m_initialRotation(rotation), m_range(range), m_halfWidth(halfWidth) {

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

    //return m_initialView;

    // TODO: This is too slow...
    // TODO: Make sure this is called at the appropriate times... by the algo or the sim???

    // First, get the edge of the view of the sensor
    std::vector<Cartesian> edge;
    for (float i = -1; i <= 1; i += 2.0/(P()->numberOfSensorEdgePoints() - 1)) {
        edge.push_back(currentPosition + Polar(m_range, currentRotation + (m_halfWidth * i)));
    }

    // For each tile the within the range of the sensor...
    for (const Tile* tile : getTilesInRange(currentPosition, currentRotation, maze)) {

        // ...iterate through all of the tile's polygons...
        for (std::vector<Polygon> group : {tile->getActualWallPolygons(), tile->getCornerPolygons()}) {
            for (Polygon obstacle : group) {
                for (std::pair<Cartesian, Cartesian> A : getLineSegments(obstacle)) {

                    // ...and check for intersections with the view
                    for (int i = 0; i < edge.size(); i += 1) {
                        std::pair<Cartesian, Cartesian> B = std::make_pair(currentPosition, edge.at(i));
                        if (linesIntersect(A, B)) {
                            edge.at(i) = getIntersectionPoint(A, B);
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

std::vector<const Tile*> Sensor::getTilesInRange(const Cartesian& currentPosition, const Radians& currentRotation,
        const Maze& maze) const {

    // TODO: Base this on the range and currentRotation

    // Get the tile containing the current position
    int px = static_cast<int>(floor((currentPosition.getX() / (P()->wallLength() + P()->wallWidth())).getMeters()));
    int py = static_cast<int>(floor((currentPosition.getY() / (P()->wallLength() + P()->wallWidth())).getMeters()));

    // Find all of the tiles that the mouse could be contained within
    std::vector<const Tile*> tilesInRange;
    for (int x = -2; x <= 2; x += 1) {
        for (int y = -2; y <= 2; y += 1) {
            if (0 <= px + x && px + x < maze.getWidth() && 0 <= py + y && py + y <= maze.getHeight()) {
                tilesInRange.push_back(maze.getTile(px + x, py + y));
            }
        }
    }

    return tilesInRange;
}

} // namespace sim

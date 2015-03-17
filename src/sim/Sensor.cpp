#include "Sensor.h"

#include <algorithm>
#include <cmath>

#include <Polar.h>

#include <limits> // TODO
#include <iostream> // TODO

#include "GeometryUtilities.h"
#include "Param.h"

namespace sim {

Sensor::Sensor(const Coordinate& position, const Distance& radius, const Angle& rotation, const Distance& range,
        const Angle& halfWidth, const Time& readTime) :
        m_initialTranslation(position), m_initialRotation(rotation), m_range(range), m_halfWidth(halfWidth),
        m_readTime(readTime) {

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
        for (const Tile* tile : lineSegmentTileCover(currentPosition, edge.at(i), maze)) {

            // ... iterate through all of the tile's polygons ...
            for (std::vector<Polygon> group : {tile->getActualWallPolygons(), tile->getCornerPolygons()}) {
                for (Polygon obstacle : group) {
                    for (std::pair<Cartesian, Cartesian> A : getLineSegments(obstacle)) {

                        // ... and check for intersections
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

/*
std::vector<const Tile*> Sensor::getTilesInRange(const Cartesian& currentPosition, const Cartesian& edgePoint,
        const Maze& maze) const {

    // TODO: Ray tracing function
    // The set of tiles consists of all Tiles that have an x-intercept or a y-intercept

    // The output vector
    std::vector<const Tile*> tilesInRange;

    // The length of any one tile
    Meters tileLength = P()->wallLength() + P()->wallWidth();

    // Lexicographical ordering
    Cartesian p0 = std::min(currentPosition, edgePoint);
    Cartesian p1 = std::max(currentPosition, edgePoint);

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
*/

Seconds Sensor::getReadTime() const {
    return m_readTime;
}

} // namespace sim

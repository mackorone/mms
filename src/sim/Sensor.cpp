#include "Sensor.h"

#include <cmath>
#include <Polar.h>
#include "Param.h"

namespace sim {

Sensor::Sensor(const Coordinate& position, const Distance& radius, const Angle& rotation, const Distance& range,
        const Angle& halfWidth) :
        m_initialTranslation(position), m_initialRotation(rotation), m_range(range), m_halfWidth(halfWidth) {

    // Create the polygon for the body of the sensor
    std::vector<Cartesian> polygon;
    for (int i = 1; i <= P()->circleApproximationEdges(); i += 1) {
        float radians = i*2*M_PI/P()->circleApproximationEdges();
        polygon.push_back(Polar(radius, Radians(radians)) + position);
    }
    m_initialPolygon = Polygon(polygon);

    // Create the polygon for the view of the sensor
    std::vector<Cartesian> view;
    view.push_back(position);
    for (int i = -1*(P()->circleApproximationEdges()/2); i <= P()->circleApproximationEdges()/2; i += 1) {
        float radians = i*2*halfWidth.getRadians()/P()->circleApproximationEdges();
        view.push_back(Polar(range, Radians(radians) + rotation) + position);
    }
    m_initialView = Polygon(view);

}

float Sensor::read(const Cartesian& translation, const Radians& rotation, const Maze& maze) {
    // TODO: I'm given the translation and rotation of the sensor
    // TODO: I need to sweep from left to right and note any intersections... resolution of the sweep???
    return 0.0;
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

} // namespace sim

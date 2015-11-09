#include "Mouse.h"

#include "units/Meters.h"
#include "units/MetersPerSecond.h"
#include "units/Polar.h"

#include "Assert.h"
#include "CPMath.h"
#include "Directory.h"
#include "GeometryUtilities.h"
#include "MouseParser.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"

namespace sim {

Mouse::Mouse(const Maze* maze) : m_maze(maze), m_gyro(RadiansPerSecond(0.0)) {
}

bool Mouse::initialize(const std::string& mouseFile) {

    // TODO: MACK If fails, return false

    // Create the mouse parser object
    MouseParser parser(Directory::getResMouseDirectory() + mouseFile);

    // First, determine the center of the starting tile
    double halfOfTileDistance = (P()->wallLength() + P()->wallWidth()) / 2.0;
    Cartesian centerOfStartingTile = Cartesian(Meters(halfOfTileDistance), Meters(halfOfTileDistance));

    // The mouse always begins at the center of the starting tile
    m_initialTranslation = centerOfStartingTile;
    m_translation = m_initialTranslation;

    // Determine the translation necessary to actually put the center of mass
    // of the mouse in the center of the starting tile
    Cartesian centerOfMass = parser.getCenterOfMass();
    Cartesian translationToAlignCenters = centerOfStartingTile - centerOfMass;

    // Initialize the body, wheels, and sensors
    m_initialBodyPolygon = parser.getBody(translationToAlignCenters);
    m_wheels = parser.getWheels(translationToAlignCenters);
    m_sensors = parser.getSensors(translationToAlignCenters);

    // Set the rotation the mouse based on the forward direction and the desired starting direction
    m_rotation = DIRECTION_TO_ANGLE.at(STRING_TO_DIRECTION.at(P()->mouseStartingDirection())) - parser.getForwardDirection();

    // Initialize the collision polygon; this is technically not correct since
    // we should be using union, not convexHull, but it's a good approximation
    std::vector<Polygon> polygons;
    polygons.push_back(m_initialBodyPolygon);
    for (std::pair<std::string, Wheel> pair : m_wheels) {
        polygons.push_back(pair.second.getInitialPolygon());
    }
    for (std::pair<std::string, Sensor> pair : m_sensors) {
        polygons.push_back(pair.second.getInitialPolygon());
    }
    m_initialCollisionPolygon = GeometryUtilities::convexHull(polygons);

    // Initialize the center of mass polygon
    m_initialCenterOfMassPolygon = Polygon::createCirclePolygon(m_initialTranslation, Meters(.005), 5);

    // Return success
    return true;
}

Polygon Mouse::getBodyPolygon() const {
    return m_initialBodyPolygon
        .translate(m_translation - m_initialTranslation)
        .rotateAroundPoint(m_rotation, m_translation);
}

Polygon Mouse::getCollisionPolygon() const {
    return m_initialCollisionPolygon
        .translate(m_translation - m_initialTranslation)
        .rotateAroundPoint(m_rotation, m_translation);
}

Polygon Mouse::getCenterOfMassPolygon() const {
    return m_initialCenterOfMassPolygon
        .translate(m_translation - m_initialTranslation)
        .rotateAroundPoint(m_rotation + Degrees(90), m_translation);
}

std::vector<Polygon> Mouse::getWheelPolygons() const {
    return getPolygonsFromMap(m_wheels);
}

std::vector<Polygon> Mouse::getWheelSpeedIndicatorPolygons() const {
    std::vector<Polygon> polygons;
    for (std::pair<std::string, Wheel> pair : m_wheels) {
        polygons.push_back(pair.second.getSpeedIndicatorPolygon()
            .translate(m_translation - m_initialTranslation)
            .rotateAroundPoint(m_rotation, m_translation));
    }
    return polygons;
}

std::vector<Polygon> Mouse::getSensorPolygons() const {
    return getPolygonsFromMap(m_sensors);
}

std::vector<Polygon> Mouse::getViewPolygons() const {

    // Get the current view for the all of the sensors
    std::vector<Polygon> polygons;
    for (std::pair<std::string, Sensor> pair : m_sensors) {
        Polygon adjusted = pair.second.getInitialView()
            .translate(m_translation - m_initialTranslation)
            .rotateAroundPoint(m_rotation, m_translation);
        polygons.push_back(pair.second.getCurrentView(
            adjusted.getVertices().at(0), m_rotation + pair.second.getInitialDirection(), *m_maze));
    }
    return polygons;
}

void Mouse::update(const Duration& elapsed) {

    // Atomically retrieve the current angular velocities of the wheels
    std::map<std::string, RadiansPerSecond> wheelAngularVelocities;
    m_wheelMutex.lock();
    for (std::pair<std::string, Wheel> wheel : m_wheels) {
        wheelAngularVelocities.insert(std::make_pair(wheel.first, wheel.second.getAngularVelocity()));
    }
    m_wheelMutex.unlock();
    
    MetersPerSecond sumDx(0);
    MetersPerSecond sumDy(0);
    RadiansPerSecond sumDr(0);

    for (std::pair<std::string, Wheel> wheel : m_wheels) {

        double radius = wheel.second.getDiameter().getMeters() / 2.0;
        MetersPerSecond linearVelocity(
            wheelAngularVelocities.at(wheel.first).getRadiansPerSecond() * radius);
        MetersPerSecond dx = linearVelocity * (m_rotation + wheel.second.getInitialDirection()).getCos();
        MetersPerSecond dy = linearVelocity * (m_rotation + wheel.second.getInitialDirection()).getSin();

        Cartesian wheelToCenter = m_initialTranslation - wheel.second.getInitialPosition();
        RadiansPerSecond dr(
            linearVelocity.getMetersPerSecond() / wheelToCenter.getRho().getMeters()
            * (wheelToCenter.getTheta() - wheel.second.getInitialDirection()).getSin());

        sumDx += dx;
        sumDy += dy;
        sumDr += dr;
    }

    MetersPerSecond aveDx = sumDx / static_cast<double>(m_wheels.size());
    MetersPerSecond aveDy = sumDy / static_cast<double>(m_wheels.size());
    RadiansPerSecond aveDr = sumDr / static_cast<double>(m_wheels.size());

    // Update the amount each wheel has rotated
    // TODO: MACK

    m_gyro = aveDr;
    m_rotation += Radians(aveDr * elapsed);
    m_translation += Cartesian(aveDx * elapsed, aveDy * elapsed);
}

bool Mouse::hasWheel(const std::string& name) const {
    return SimUtilities::mapContains(m_wheels, name);
}

void Mouse::setWheelSpeeds(const std::map<std::string, RadiansPerSecond>& wheelSpeeds) {
    m_wheelMutex.lock();
    for (std::pair<std::string, RadiansPerSecond> pair : wheelSpeeds) {
        ASSERT_TR(SimUtilities::mapContains(m_wheels, pair.first));
        m_wheels.at(pair.first).setAngularVelocity(pair.second);
    }
    m_wheelMutex.unlock();
}

bool Mouse::hasSensor(const std::string& name) const {
    return SimUtilities::mapContains(m_sensors, name);
}

double Mouse::readSensor(const std::string& name) const {

    // Validate the input
    ASSERT_TR(hasSensor(name));
    Sensor sensor = m_sensors.at(name);

    // Retrieve the current translation and rotation
    Cartesian currentTranslation = m_translation;
    Radians currentRotation = m_rotation;

    // Determine the reading
    Polygon fullView = sensor.getInitialView()
        .translate(currentTranslation - m_initialTranslation)
        .rotateAroundPoint(currentRotation, currentTranslation);
    Polygon currentView = sensor.getCurrentView(
        fullView.getVertices().at(0), currentRotation + sensor.getInitialDirection(), *m_maze);
    return 1.0 - currentView.area() / fullView.area();
}

Seconds Mouse::getSensorReadDuration(const std::string& name) const {
    ASSERT_TR(SimUtilities::mapContains(m_sensors, name));
    return m_sensors.at(name).getReadDuration();
}

RadiansPerSecond Mouse::readGyro() const {
    return m_gyro;
}

std::pair<int, int> Mouse::getDiscretizedTranslation() const {
    int x = static_cast<int>(floor((getCurrentTranslation().getX() / Meters(P()->wallLength() + P()->wallWidth()))));
    int y = static_cast<int>(floor((getCurrentTranslation().getY() / Meters(P()->wallLength() + P()->wallWidth()))));
    return std::make_pair(x, y);
}

Direction Mouse::getDiscretizedRotation() const {
    int dir = static_cast<int>(floor((getCurrentRotation() + Degrees(45)) / Degrees(90)));
    switch (dir) {
        case 0:
            return Direction::NORTH;
        case 1:
            return Direction::WEST;
        case 2:
            return Direction::SOUTH;
        case 3:
            return Direction::EAST;
    }
}

Cartesian Mouse::getInitialTranslation() const {
    return m_initialTranslation;
}

Cartesian Mouse::getCurrentTranslation() const {
    return m_translation;
}

Radians Mouse::getCurrentRotation() const {
    return m_rotation;
}

void Mouse::teleport(const Cartesian& translation, const Angle& rotation) {
    m_translation = translation;
    m_rotation = rotation;
}

} // namespace sim

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

#include "Logging.h" // TODO: MACK

namespace sim {

Mouse::Mouse(const Maze* maze) : m_maze(maze), m_currentGyro(RadiansPerSecond(0.0)) {
}

bool Mouse::initialize(const std::string& mouseFile) {

    // TODO: MACK If fails, return false

    // The initial translation of the mouse is just the center of the starting tile
    Meters halfOfTileDistance = Meters((P()->wallLength() + P()->wallWidth()) / 2.0);
    m_initialTranslation = Cartesian(halfOfTileDistance, halfOfTileDistance);
    m_currentTranslation = m_initialTranslation;

    // The initial rotation of the mouse, however, is determined by the parameters
    m_initialRotation = DIRECTION_TO_ANGLE.at(STRING_TO_DIRECTION.at(P()->mouseStartingDirection()));
    m_currentRotation = m_initialRotation;

    // Create the mouse parser object
    MouseParser parser(Directory::getResMouseDirectory() + mouseFile);

    // Initialize the body, wheels, and sensors, such that they have the
    // correct initial translation and rotation
    m_bodyPolygon = parser.getBody(m_initialTranslation, m_initialRotation);
    m_wheels = parser.getWheels(m_initialTranslation, m_initialRotation);
    m_sensors = parser.getSensors(m_initialTranslation, m_initialRotation);

    // Initialize the collision polygon; this is technically not correct since
    // we should be using union, not convexHull, but it's a good approximation
    std::vector<Polygon> polygons;
    polygons.push_back(m_bodyPolygon);
    for (std::pair<std::string, Wheel> pair : m_wheels) {
        polygons.push_back(pair.second.getInitialPolygon());
    }
    for (std::pair<std::string, Sensor> pair : m_sensors) {
        polygons.push_back(pair.second.getInitialPolygon());
    }
    m_collisionPolygon = GeometryUtilities::convexHull(polygons);

    // Initialize the center of mass polygon
    m_centerOfMassPolygon = GeometryUtilities::createCirclePolygon(m_initialTranslation, Meters(.005), 8);

    // Return success
    return true;
}

Cartesian Mouse::getInitialTranslation() const {
    return m_initialTranslation;
}

Radians Mouse::getInitialRotation() const {
    return m_initialRotation;
}

std::pair<Cartesian, Radians> Mouse::getCurrentTranslationAndRotation() const {
    return std::make_pair(m_currentTranslation, m_currentRotation);
}

void Mouse::teleport(const Coordinate& translation, const Angle& rotation) {
    m_currentTranslation = translation;
    m_currentRotation = rotation;
}

Polygon Mouse::getCurrentBodyPolygon(
        const Coordinate& currentTranslation, const Angle& currentRotation) const {
    return getCurrentPolygon(m_bodyPolygon, currentTranslation, currentRotation);
}

Polygon Mouse::getCurrentCollisionPolygon(
        const Coordinate& currentTranslation, const Angle& currentRotation) const {
    return getCurrentPolygon(m_collisionPolygon, currentTranslation, currentRotation);
}

Polygon Mouse::getCurrentCenterOfMassPolygon(
        const Coordinate& currentTranslation, const Angle& currentRotation) const {
    return getCurrentPolygon(m_centerOfMassPolygon, currentTranslation, currentRotation);
}

std::vector<Polygon> Mouse::getCurrentWheelPolygons(
        const Coordinate& currentTranslation, const Angle& currentRotation) const {
    std::vector<Polygon> polygons;
    for (std::pair<std::string, Wheel> pair : m_wheels) {
        polygons.push_back(getCurrentPolygon(pair.second.getInitialPolygon(), currentTranslation, currentRotation));
    }
    return polygons;
}

std::vector<Polygon> Mouse::getCurrentWheelSpeedIndicatorPolygons(
        const Coordinate& currentTranslation, const Angle& currentRotation) const {
    std::vector<Polygon> polygons;
    for (std::pair<std::string, Wheel> pair : m_wheels) {
        polygons.push_back(getCurrentPolygon(pair.second.getSpeedIndicatorPolygon(), currentTranslation, currentRotation));
    }
    return polygons;
}

std::vector<Polygon> Mouse::getCurrentSensorPolygons(
        const Coordinate& currentTranslation, const Angle& currentRotation) const {
    std::vector<Polygon> polygons;
    for (std::pair<std::string, Sensor> pair : m_sensors) {
        polygons.push_back(getCurrentPolygon(pair.second.getInitialPolygon(), currentTranslation, currentRotation));
    }
    return polygons;
}

std::vector<Polygon> Mouse::getCurrentViewPolygons(
        const Coordinate& currentTranslation, const Angle& currentRotation) const {
    std::vector<Polygon> polygons;
    for (std::pair<std::string, Sensor> pair : m_sensors) {
        Polygon adjusted = getCurrentPolygon(pair.second.getInitialView(), currentTranslation, currentRotation);
        polygons.push_back(pair.second.getCurrentView(
            adjusted.getVertices().at(0), pair.second.getInitialDirection() + currentRotation, *m_maze));
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

    std::pair<Cartesian, Radians> currentTranslationAndRotation = getCurrentTranslationAndRotation();
    Cartesian currentTranslation = currentTranslationAndRotation.first;
    Radians currentRotation = currentTranslationAndRotation.second;
    
    MetersPerSecond sumDx(0);
    MetersPerSecond sumDy(0);
    RadiansPerSecond sumDr(0);

    for (std::pair<std::string, Wheel> wheel : m_wheels) {

        MetersPerSecond linearVelocity = wheelAngularVelocities.at(wheel.first) * wheel.second.getRadius();
        MetersPerSecond dx = linearVelocity * (currentRotation - getInitialRotation() + wheel.second.getInitialDirection()).getCos(); // TODO: MACK
        MetersPerSecond dy = linearVelocity * (currentRotation - getInitialRotation() + wheel.second.getInitialDirection()).getSin(); // TODO: MACK

        Cartesian wheelToCenter = getInitialTranslation() - wheel.second.getInitialPosition();
        double rotationFactor = (wheelToCenter.getTheta() - wheel.second.getInitialDirection()).getSin();
        RadiansPerSecond dr = RadiansPerSecond(
            linearVelocity.getMetersPerSecond() / wheelToCenter.getRho().getMeters() * rotationFactor);

        sumDx += dx;
        sumDy += dy;
        sumDr += dr;
    }

    MetersPerSecond aveDx = sumDx / static_cast<double>(m_wheels.size());
    MetersPerSecond aveDy = sumDy / static_cast<double>(m_wheels.size());
    RadiansPerSecond aveDr = sumDr / static_cast<double>(m_wheels.size());

    // Update the amount each wheel has rotated
    // TODO: MACK

    m_currentGyro = aveDr;
    m_currentRotation += Radians(aveDr * elapsed);
    m_currentTranslation += Cartesian(aveDx * elapsed, aveDy * elapsed);
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
    std::pair<Cartesian, Radians> currentTranslationAndRotation = getCurrentTranslationAndRotation();
    Cartesian currentTranslation = currentTranslationAndRotation.first;
    Radians currentRotation = currentTranslationAndRotation.second;

    // Determine the reading
    Polygon fullView = sensor.getInitialView()
        .translate(currentTranslation - getInitialTranslation())
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
    return m_currentGyro;
}

// TODO: MACK - What to do about these...
std::pair<int, int> Mouse::getDiscretizedTranslation() const {
    Cartesian currentTranslation = getCurrentTranslationAndRotation().first;
    int x = static_cast<int>(floor((currentTranslation.getX() / Meters(P()->wallLength() + P()->wallWidth()))));
    int y = static_cast<int>(floor((currentTranslation.getY() / Meters(P()->wallLength() + P()->wallWidth()))));
    return std::make_pair(x, y);
}

Direction Mouse::getDiscretizedRotation() const {
    Radians currentRotation = getCurrentTranslationAndRotation().second;
    int dir = static_cast<int>(floor((currentRotation + Degrees(45)) / Degrees(90)));
    switch (dir) {
        case 0:
            return Direction::EAST;
        case 1:
            return Direction::NORTH;
        case 2:
            return Direction::WEST;
        case 3:
            return Direction::SOUTH;
    }
}

Polygon Mouse::getCurrentPolygon(const Polygon& initialPolygon,
        const Cartesian& currentTranslation, const Radians& currentRotation) const {
    return initialPolygon
        .translate(currentTranslation - getInitialTranslation())
        .rotateAroundPoint(currentRotation - getInitialRotation(), currentTranslation);
}

} // namespace sim

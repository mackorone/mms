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
    m_initialBodyPolygon = parser.getBody(m_initialTranslation, m_initialRotation);
    m_wheels = parser.getWheels(m_initialTranslation, m_initialRotation);
    m_sensors = parser.getSensors(m_initialTranslation, m_initialRotation);

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
    m_initialCenterOfMassPolygon = GeometryUtilities::createCirclePolygon(m_initialTranslation, Meters(.005), 8);

    // Return success
    return true;
}

Cartesian Mouse::getInitialTranslation() const {
    return m_initialTranslation;
}

Radians Mouse::getInitialRotation() const {
    return m_initialRotation;
}

Cartesian Mouse::getCurrentTranslation() const {
    return m_currentTranslation;
}

Radians Mouse::getCurrentRotation() const {
    return m_currentRotation;
}

std::pair<int, int> Mouse::getCurrentDiscretizedTranslation() const {
    Cartesian currentTranslation = getCurrentTranslation();
    int x = static_cast<int>(floor((currentTranslation.getX() / Meters(P()->wallLength() + P()->wallWidth()))));
    int y = static_cast<int>(floor((currentTranslation.getY() / Meters(P()->wallLength() + P()->wallWidth()))));
    return std::make_pair(x, y);
}

Direction Mouse::getCurrentDiscretizedRotation() const {
    int dir = static_cast<int>(floor((getCurrentRotation() + Degrees(45)) / Degrees(90)));
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

void Mouse::teleport(const Coordinate& translation, const Angle& rotation) {
    m_currentTranslation = translation;
    m_currentRotation = rotation;
}

Polygon Mouse::getCurrentBodyPolygon(
        const Coordinate& currentTranslation, const Angle& currentRotation) const {
    return getCurrentPolygon(m_initialBodyPolygon, currentTranslation, currentRotation);
}

Polygon Mouse::getCurrentCollisionPolygon(
        const Coordinate& currentTranslation, const Angle& currentRotation) const {
    return getCurrentPolygon(m_initialCollisionPolygon, currentTranslation, currentRotation);
}

Polygon Mouse::getCurrentCenterOfMassPolygon(
        const Coordinate& currentTranslation, const Angle& currentRotation) const {
    return getCurrentPolygon(m_initialCenterOfMassPolygon, currentTranslation, currentRotation);
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

std::vector<Polygon> Mouse::getCurrentSensorViewPolygons(
        const Coordinate& currentTranslation, const Angle& currentRotation) const {
    std::vector<Polygon> polygons;
    for (std::pair<std::string, Sensor> pair : m_sensors) {
        polygons.push_back(getCurrentSensorViewPolygon(pair.second, currentTranslation, currentRotation));
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

        MetersPerSecond linearVelocity = wheelAngularVelocities.at(wheel.first) * wheel.second.getRadius();
        MetersPerSecond dx = linearVelocity *
            (getCurrentRotation() - getInitialRotation() + wheel.second.getInitialDirection()).getCos();
        MetersPerSecond dy = linearVelocity *
            (getCurrentRotation() - getInitialRotation() + wheel.second.getInitialDirection()).getSin();

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

    m_currentGyro = aveDr;
    m_currentRotation += Radians(aveDr * elapsed);
    m_currentTranslation += Cartesian(aveDx * elapsed, aveDy * elapsed);
}

bool Mouse::hasWheel(const std::string& name) const {
    return SimUtilities::mapContains(m_wheels, name);
}

RadiansPerSecond Mouse::getWheelMaxSpeed(const std::string& name) const {
    ASSERT_TR(SimUtilities::mapContains(m_wheels, name));
    return m_wheels.at(name).getMaxAngularVelocityMagnitude();
}

void Mouse::setWheelSpeeds(const std::map<std::string, RadiansPerSecond>& wheelSpeeds) {
    m_wheelMutex.lock();
    for (std::pair<std::string, RadiansPerSecond> pair : wheelSpeeds) {
        ASSERT_TR(SimUtilities::mapContains(m_wheels, pair.first));
        ASSERT_LE(
            std::abs(pair.second.getRevolutionsPerSecond()),
            getWheelMaxSpeed(pair.first).getRevolutionsPerSecond());
        m_wheels.at(pair.first).setAngularVelocity(pair.second);
    }
    m_wheelMutex.unlock();
}

void Mouse::setWheelSpeedsForMoveForward() {
    std::map<std::string, RadiansPerSecond> wheelSpeeds;
    for (std::pair<std::string, Wheel> wheel : m_wheels) {
        wheelSpeeds.insert(std::make_pair(wheel.first,
            getWheelMaxSpeed(wheel.first) * getWheelContributionFactors(wheel.first).first));
    }
    setWheelSpeeds(wheelSpeeds);
}

void Mouse::setWheelSpeedsForTurnLeft() {
    std::map<std::string, RadiansPerSecond> wheelSpeeds;
    for (std::pair<std::string, Wheel> wheel : m_wheels) {
        wheelSpeeds.insert(std::make_pair(wheel.first,
            getWheelMaxSpeed(wheel.first) * getWheelContributionFactors(wheel.first).second));
    }
    setWheelSpeeds(wheelSpeeds);
}

void Mouse::setWheelSpeedsForTurnRight() {
    std::map<std::string, RadiansPerSecond> wheelSpeeds;
    for (std::pair<std::string, Wheel> wheel : m_wheels) {
        wheelSpeeds.insert(std::make_pair(wheel.first,
            getWheelMaxSpeed(wheel.first) * getWheelContributionFactors(wheel.first).second * -1));
    }
    setWheelSpeeds(wheelSpeeds);
}

void Mouse::stopAllWheels() {
    std::map<std::string, RadiansPerSecond> wheelSpeeds;
    for (std::pair<std::string, Wheel> wheel : m_wheels) {
        wheelSpeeds.insert(std::make_pair(wheel.first, RadiansPerSecond(0)));
    }
    setWheelSpeeds(wheelSpeeds);
}

bool Mouse::hasSensor(const std::string& name) const {
    return SimUtilities::mapContains(m_sensors, name);
}

double Mouse::readSensor(const std::string& name) const {
    ASSERT_TR(hasSensor(name));
    Sensor sensor = m_sensors.at(name);
    Polygon currentView = getCurrentSensorViewPolygon(
        sensor, getCurrentTranslation(), getCurrentRotation());
    return 1.0 - currentView.area() / sensor.getInitialViewPolygon().area();
}

Seconds Mouse::getSensorReadDuration(const std::string& name) const {
    ASSERT_TR(SimUtilities::mapContains(m_sensors, name));
    return m_sensors.at(name).getReadDuration();
}

RadiansPerSecond Mouse::readGyro() const {
    return m_currentGyro;
}

Polygon Mouse::getCurrentPolygon(const Polygon& initialPolygon,
        const Cartesian& currentTranslation, const Radians& currentRotation) const {
    return initialPolygon
        .translate(currentTranslation - getInitialTranslation())
        .rotateAroundPoint(currentRotation - getInitialRotation(), currentTranslation);
}

Polygon Mouse::getCurrentSensorViewPolygon(const Sensor& sensor,
        const Cartesian& currentTranslation, const Radians& currentRotation) const {
    Cartesian translationDelta = currentTranslation - getInitialTranslation();
    Radians rotationDelta = currentRotation - getInitialRotation();
    return sensor.getCurrentViewPolygon(
        GeometryUtilities::rotateVertexAroundPoint(
            GeometryUtilities::translateVertex(
                sensor.getInitialPosition(),
                translationDelta),
            rotationDelta,
            currentTranslation),
        sensor.getInitialDirection() + rotationDelta,
        *m_maze);
}

std::pair<double, double> Mouse::getWheelContributionFactors(const std::string& name) const {

    static std::map<std::string, std::pair<double, double>> contributionFactors;

    if (contributionFactors.empty()) {

        MetersPerSecond maxForwardContributionMagnitude(0);
        RadiansPerSecond maxRadialContributionMagnitude(0);
        std::map<std::string, std::pair<MetersPerSecond, RadiansPerSecond>> contributionPairs;

        for (std::pair<std::string, Wheel> wheel : m_wheels) {

            MetersPerSecond maxLinearVelocity = wheel.second.getMaxAngularVelocityMagnitude() * wheel.second.getRadius();
            MetersPerSecond forwardContribution = maxLinearVelocity *
                (getInitialRotation() - wheel.second.getInitialDirection()).getCos();

            Cartesian wheelToCenter = getInitialTranslation() - wheel.second.getInitialPosition();
            double rotationFactor = (wheelToCenter.getTheta() - wheel.second.getInitialDirection()).getSin();
            RadiansPerSecond radialContribuition = RadiansPerSecond(
                maxLinearVelocity.getMetersPerSecond() / wheelToCenter.getRho().getMeters() * rotationFactor);

            contributionPairs.insert(std::make_pair(wheel.first, std::make_pair(forwardContribution, radialContribuition)));

            MetersPerSecond forwardContributionMagnitude(std::abs(forwardContribution.getMetersPerSecond()));
            RadiansPerSecond radialContributionMagnitude(std::abs(radialContribuition.getRadiansPerSecond()));
            if (maxForwardContributionMagnitude < forwardContributionMagnitude) {
                maxForwardContributionMagnitude = forwardContributionMagnitude;
            }
            if (maxRadialContributionMagnitude < radialContributionMagnitude) {
                maxRadialContributionMagnitude = radialContributionMagnitude;
            }
        }

        for (std::pair<std::string, std::pair<MetersPerSecond, RadiansPerSecond>> pair : contributionPairs) {
            contributionFactors.insert(std::make_pair(pair.first,
                std::make_pair(
                    pair.second.first / maxForwardContributionMagnitude,
                    pair.second.second / maxRadialContributionMagnitude)));
        }
    }
    
    ASSERT_TR(SimUtilities::mapContains(contributionFactors, name));
    return contributionFactors.at(name);
}

} // namespace sim

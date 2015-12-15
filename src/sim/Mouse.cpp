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

bool Mouse::initialize(
        const std::string& mouseFile,
        Direction initialDirection) {

    // We begin with the assumption that the initialization will succeed
    bool success = true;

    // The initial translation of the mouse is just the center of the starting tile
    Meters halfOfTileDistance = Meters((P()->wallLength() + P()->wallWidth()) / 2.0);
    m_initialTranslation = Cartesian(halfOfTileDistance, halfOfTileDistance);
    m_currentTranslation = m_initialTranslation;

    // The initial rotation of the mouse, however, is determined by the options
    m_initialRotation = DIRECTION_TO_ANGLE.at(initialDirection);
    m_currentRotation = m_initialRotation;

    // Create the mouse parser object
    MouseParser parser(Directory::getResMouseDirectory() + mouseFile, &success);
    if (!success) { // A checkpoint so that we can fail faster
        return false;
    }

    // Initialize the body, wheels, and sensors, such that they have the
    // correct initial translation and rotation
    m_initialBodyPolygon = parser.getBody(m_initialTranslation, m_initialRotation, &success);
    m_wheels = parser.getWheels(m_initialTranslation, m_initialRotation, &success);
    m_sensors = parser.getSensors(m_initialTranslation, m_initialRotation, &success);

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
    return success;
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
    int x = static_cast<int>(std::floor(
        currentTranslation.getX() / Meters(P()->wallLength() + P()->wallWidth())
    ));
    int y = static_cast<int>(std::floor(
        currentTranslation.getY() / Meters(P()->wallLength() + P()->wallWidth())
    ));
    return std::make_pair(x, y);
}

Direction Mouse::getCurrentDiscretizedRotation() const {
    int dir = static_cast<int>(std::floor(
        (getCurrentRotation() + Degrees(45)).getRadiansZeroTo2pi() /
        Degrees(90).getRadiansZeroTo2pi()
    ));
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

    m_updateMutex.lock();

    MetersPerSecond sumDx(0);
    MetersPerSecond sumDy(0);
    RadiansPerSecond sumDr(0);

    for (const std::pair<std::string, Wheel>& wheel : m_wheels) {

        m_wheels.at(wheel.first).updateRotation(wheel.second.getAngularVelocity() * elapsed);

        MetersPerSecond linearVelocity = wheel.second.getAngularVelocity() * wheel.second.getRadius();
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

    m_updateMutex.unlock();
}

bool Mouse::hasWheel(const std::string& name) const {
    return SimUtilities::mapContains(m_wheels, name);
}

RadiansPerSecond Mouse::getWheelMaxSpeed(const std::string& name) const {
    ASSERT_TR(SimUtilities::mapContains(m_wheels, name));
    return m_wheels.at(name).getMaxAngularVelocityMagnitude();
}

void Mouse::setWheelSpeeds(const std::map<std::string, RadiansPerSecond>& wheelSpeeds) {
    m_updateMutex.lock();
    for (std::pair<std::string, RadiansPerSecond> pair : wheelSpeeds) {
        ASSERT_TR(SimUtilities::mapContains(m_wheels, pair.first));
        ASSERT_LE(
            std::abs(pair.second.getRevolutionsPerMinute()),
            getWheelMaxSpeed(pair.first).getRevolutionsPerMinute());
        m_wheels.at(pair.first).setAngularVelocity(pair.second);
    }
    m_updateMutex.unlock();
}

void Mouse::setWheelSpeedsForMoveForward(double fractionOfMaxSpeed) {
    std::map<std::string, RadiansPerSecond> wheelSpeeds;
    for (std::pair<std::string, Wheel> wheel : m_wheels) {
        wheelSpeeds.insert(std::make_pair(wheel.first,
            getWheelMaxSpeed(wheel.first) * fractionOfMaxSpeed *
            getWheelContributionFactors(wheel.first).first));
    }
    setWheelSpeeds(wheelSpeeds);
}

void Mouse::setWheelSpeedsForTurnLeft(double fractionOfMaxSpeed) {
    std::map<std::string, RadiansPerSecond> wheelSpeeds;
    for (std::pair<std::string, Wheel> wheel : m_wheels) {
        wheelSpeeds.insert(std::make_pair(wheel.first,
            getWheelMaxSpeed(wheel.first) * fractionOfMaxSpeed *
            getWheelContributionFactors(wheel.first).second));
    }
    setWheelSpeeds(wheelSpeeds);
}

void Mouse::setWheelSpeedsForTurnRight(double fractionOfMaxSpeed) {
    std::map<std::string, RadiansPerSecond> wheelSpeeds;
    for (std::pair<std::string, Wheel> wheel : m_wheels) {
        wheelSpeeds.insert(std::make_pair(wheel.first,
            getWheelMaxSpeed(wheel.first) * fractionOfMaxSpeed *
            getWheelContributionFactors(wheel.first).second * -1));
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

EncoderType Mouse::getWheelEncoderType(const std::string& name) const {
    ASSERT_TR(hasWheel(name));
    return m_wheels.at(name).getEncoderType();
}

double Mouse::getWheelEncoderTicksPerRevolution(const std::string& name) const {
    ASSERT_TR(hasWheel(name));
    return m_wheels.at(name).getEncoderTicksPerRevolution();
}

int Mouse::readWheelAbsoluteEncoder(const std::string& name) {
    ASSERT_TR(hasWheel(name));
    m_updateMutex.lock();
    int encoderReading = m_wheels.at(name).readAbsoluteEncoder();
    m_updateMutex.unlock();
    return encoderReading;
}

int Mouse::readWheelRelativeEncoder(const std::string& name) {
    ASSERT_TR(hasWheel(name));
    m_updateMutex.lock();
    int encoderReading = m_wheels.at(name).readRelativeEncoder();
    m_updateMutex.unlock();
    return encoderReading;
}

void Mouse::resetWheelRelativeEncoder(const std::string& name) {
    ASSERT_TR(hasWheel(name));
    m_updateMutex.lock();
    m_wheels.at(name).resetRelativeEncoder();
    m_updateMutex.unlock();
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

    // TODO: upforgrabs
    // This function was implemented really hastily. If you've got the time and
    // determination, it'd be really nice if this was reimplemented and
    // deduplicated with the update() method above. This function is supposed
    // to return a pair of doubles, each in [0.0, 1.0], that specify how much
    // the wheel contributes the forward movement and rotation of the mouse.
    // This information is used by the setWheelSpeedsForMoveForward and similar
    // methods to set the appropriate wheels when a particular action is
    // requested. That is, if we've got a wheel that's facing to the right, we
    // don't want to turn that wheel when we're trying to move forward.
    // Instead, we should only turn the wheels that will actually contribute to
    // the forward movement of the mouse. I'm not yet sure if we should take
    // wheel size and/or max angular velocity magnitude into account, but I've
    // done so here. This can definitely be changed if you decide that it's
    // better without them. Also, sticking the memoization in this method is
    // sloppy. First off, it makes the funciton less readable. And second, the
    // static map means that we can't have multiple mice in the simulator. We
    // should fix this.

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
            contributionFactors.insert(
                std::make_pair(
                    pair.first,
                    std::make_pair(
                        pair.second.first / maxForwardContributionMagnitude,
                        pair.second.second / maxRadialContributionMagnitude)));
        }
    }
    
    ASSERT_TR(SimUtilities::mapContains(contributionFactors, name));
    return contributionFactors.at(name);
}

} // namespace sim

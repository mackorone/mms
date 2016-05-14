#include "Mouse.h"

#include "units/Meters.h"
#include "units/MetersPerSecond.h"
#include "units/Polar.h"

#include "Assert.h"
#include "ContainerUtilities.h"
#include "CPMath.h"
#include "Directory.h"
#include "GeometryUtilities.h"
#include "MouseParser.h"
#include "Param.h"
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
    m_sensors = parser.getSensors(m_initialTranslation, m_initialRotation, *m_maze, &success);

    // Initialize the wheel effects and speed adjustment factors
    m_wheelEffects = getWheelEffects(m_initialTranslation, m_initialRotation, m_wheels);
    m_wheelSpeedAdjustmentFactors = getWheelSpeedAdjustmentFactors(m_wheels, m_wheelEffects);

    // Initialize the curve turn factors, based on previously determined info
    m_curveTurnFactorCalculator = CurveTurnFactorCalculator(
        m_wheels,
        m_wheelEffects,
        m_wheelSpeedAdjustmentFactors);

    // Initialize the collision polygon; this is technically not correct since
    // we should be using union, not convexHull, but it's a good approximation
    std::vector<Polygon> polygons;
    polygons.push_back(m_initialBodyPolygon);
    for (const std::pair<const std::string&, const Wheel&>& pair : m_wheels) {
        polygons.push_back(pair.second.getInitialPolygon());
    }
    for (const std::pair<const std::string&, const Sensor&>& pair : m_sensors) {
        polygons.push_back(pair.second.getInitialPolygon());
    }
    m_initialCollisionPolygon = GeometryUtilities::convexHull(polygons);

    // Initialize the center of mass polygon
    m_initialCenterOfMassPolygon = GeometryUtilities::createCirclePolygon(m_initialTranslation, Meters(.005), 8);

    // Force triangulation of the drawable polygons, thus ensuring
    // that we only triangulate once, at the beginning of execution
    m_initialBodyPolygon.getTriangles();
    m_initialCollisionPolygon.getTriangles();
    m_initialCenterOfMassPolygon.getTriangles();
    for (const std::pair<const std::string&, const Wheel&>& pair : m_wheels) {
        pair.second.getInitialPolygon().getTriangles();
    }
    for (const std::pair<const std::string&, const Sensor&>& pair : m_sensors) {
        pair.second.getInitialPolygon().getTriangles();
        pair.second.getInitialViewPolygon().getTriangles();
    }

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
    static Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    Cartesian currentTranslation = getCurrentTranslation();
    int x = static_cast<int>(
        std::floor(
            currentTranslation.getX() / tileLength
        )
    );
    int y = static_cast<int>(
        std::floor(
            currentTranslation.getY() / tileLength
        )
    );
    return std::make_pair(x, y);
}

Direction Mouse::getCurrentDiscretizedRotation() const {
    int dir = static_cast<int>(
        std::floor(
            (getCurrentRotation() + Degrees(45)).getRadiansZeroTo2pi() /
            Degrees(90).getRadiansZeroTo2pi()
        )
    );
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
    for (const std::pair<const std::string&, const Wheel&>& pair : m_wheels) {
        polygons.push_back(getCurrentPolygon(pair.second.getInitialPolygon(), currentTranslation, currentRotation));
    }
    return polygons;
}

std::vector<Polygon> Mouse::getCurrentWheelSpeedIndicatorPolygons(
        const Coordinate& currentTranslation, const Angle& currentRotation) const {
    std::vector<Polygon> polygons;
    for (const std::pair<const std::string&, const Wheel&>& pair : m_wheels) {
        polygons.push_back(getCurrentPolygon(pair.second.getSpeedIndicatorPolygon(), currentTranslation, currentRotation));
    }
    return polygons;
}

std::vector<Polygon> Mouse::getCurrentSensorPolygons(
        const Coordinate& currentTranslation, const Angle& currentRotation) const {
    std::vector<Polygon> polygons;
    for (const std::pair<const std::string&, const Sensor&>& pair : m_sensors) {
        polygons.push_back(getCurrentPolygon(pair.second.getInitialPolygon(), currentTranslation, currentRotation));
    }
    return polygons;
}

std::vector<Polygon> Mouse::getCurrentSensorViewPolygons(
        const Coordinate& currentTranslation, const Angle& currentRotation) const {
    std::vector<Polygon> polygons;
    for (const std::pair<const std::string&, const Sensor&>& pair : m_sensors) {
        std::pair<Cartesian, Radians> translationAndRotation =
            getCurrentSensorPositionAndDirection(
                pair.second,
                currentTranslation,
                currentRotation);
        polygons.push_back(
            pair.second.getCurrentViewPolygon(
                translationAndRotation.first,
                translationAndRotation.second,
                *m_maze));
    }
    return polygons;
}

void Mouse::update(const Duration& elapsed) {

    // NOTE: This is a *very* performance critical function

    m_updateMutex.lock();

    MetersPerSecond sumDx(0);
    MetersPerSecond sumDy(0);
    RadiansPerSecond sumDr(0);

    for (const std::pair<const std::string&, const Wheel&>& pair : m_wheels) {

        // We can't do pair.second.updateRotation() since that breaks const
        m_wheels.at(pair.first).updateRotation(pair.second.getAngularVelocity() * elapsed);

        // Get the effects on the rate of change of translation, both forward
        // and sideways, and rotation of the mouse due to this particular wheel
        std::tuple<MetersPerSecond, MetersPerSecond, RadiansPerSecond> effects =
            m_wheelEffects.at(pair.first).getEffects(pair.second.getAngularVelocity());

        // The effect of the forward component
        sumDx += std::get<0>(effects) * getCurrentRotation().getCos();
        sumDy += std::get<0>(effects) * getCurrentRotation().getSin();

        // The effect of the sideways component
        sumDx += std::get<1>(effects) * getCurrentRotation().getSin();
        sumDy += std::get<1>(effects) * getCurrentRotation().getCos() * -1;

        // The effect of the rotation component
        sumDr += std::get<2>(effects);
    }

    MetersPerSecond aveDx = sumDx / static_cast<double>(m_wheels.size());
    MetersPerSecond aveDy = sumDy / static_cast<double>(m_wheels.size());
    RadiansPerSecond aveDr = sumDr / static_cast<double>(m_wheels.size());

    m_currentGyro = aveDr;
    m_currentRotation += Radians(aveDr * elapsed);
    m_currentTranslation += Cartesian(aveDx * elapsed, aveDy * elapsed);

    for (const std::pair<const std::string&, const Sensor&>& pair : m_sensors) {
        std::pair<Cartesian, Radians> translationAndRotation =
            getCurrentSensorPositionAndDirection(
                pair.second,
                m_currentTranslation,
                m_currentRotation);
        // We can't do pair.second.updateReading() since that breaks const
        m_sensors.at(pair.first).updateReading(
            translationAndRotation.first,
            translationAndRotation.second,
            *m_maze);
    }

    m_updateMutex.unlock();
}

bool Mouse::hasWheel(const std::string& name) const {
    return ContainerUtilities::mapContains(m_wheels, name);
}

RadiansPerSecond Mouse::getWheelMaxSpeed(const std::string& name) const {
    SIM_ASSERT_TR(ContainerUtilities::mapContains(m_wheels, name));
    return m_wheels.at(name).getMaxAngularVelocityMagnitude();
}

void Mouse::setWheelSpeeds(const std::map<std::string, RadiansPerSecond>& wheelSpeeds) {
    m_updateMutex.lock();
    for (std::pair<std::string, RadiansPerSecond> pair : wheelSpeeds) {
        SIM_ASSERT_TR(ContainerUtilities::mapContains(m_wheels, pair.first));
        SIM_ASSERT_LE(
            std::abs(pair.second.getRevolutionsPerMinute()),
            getWheelMaxSpeed(pair.first).getRevolutionsPerMinute());
        m_wheels.at(pair.first).setAngularVelocity(pair.second);
    }
    m_updateMutex.unlock();
}

void Mouse::setWheelSpeedsForMoveForward(double fractionOfMaxSpeed) {
    setWheelSpeedsForMovement(fractionOfMaxSpeed, 1.0, 0.0);
}

void Mouse::setWheelSpeedsForCurveLeft(double fractionOfMaxSpeed, const Meters& radius) {
    std::pair<double, double> curveTurnFactors = m_curveTurnFactorCalculator.getCurveTurnFactors(radius);
    setWheelSpeedsForMovement(fractionOfMaxSpeed, curveTurnFactors.first, curveTurnFactors.second);
}

void Mouse::setWheelSpeedsForCurveRight(double fractionOfMaxSpeed, const Meters& radius) {
    std::pair<double, double> curveTurnFactors = m_curveTurnFactorCalculator.getCurveTurnFactors(radius);
    setWheelSpeedsForMovement(fractionOfMaxSpeed, curveTurnFactors.first, -1 * curveTurnFactors.second);
}

void Mouse::stopAllWheels() {
    std::map<std::string, RadiansPerSecond> wheelSpeeds;
    for (const std::pair<const std::string&, const Wheel&>& wheel : m_wheels) {
        wheelSpeeds.insert(std::make_pair(wheel.first, RadiansPerSecond(0)));
    }
    setWheelSpeeds(wheelSpeeds);
}

EncoderType Mouse::getWheelEncoderType(const std::string& name) const {
    SIM_ASSERT_TR(hasWheel(name));
    return m_wheels.at(name).getEncoderType();
}

double Mouse::getWheelEncoderTicksPerRevolution(const std::string& name) const {
    SIM_ASSERT_TR(hasWheel(name));
    return m_wheels.at(name).getEncoderTicksPerRevolution();
}

int Mouse::readWheelAbsoluteEncoder(const std::string& name) const {
    SIM_ASSERT_TR(hasWheel(name));
    m_updateMutex.lock();
    int encoderReading = m_wheels.at(name).readAbsoluteEncoder();
    m_updateMutex.unlock();
    return encoderReading;
}

int Mouse::readWheelRelativeEncoder(const std::string& name) const {
    SIM_ASSERT_TR(hasWheel(name));
    m_updateMutex.lock();
    int encoderReading = m_wheels.at(name).readRelativeEncoder();
    m_updateMutex.unlock();
    return encoderReading;
}

void Mouse::resetWheelRelativeEncoder(const std::string& name) {
    SIM_ASSERT_TR(hasWheel(name));
    m_updateMutex.lock();
    m_wheels.at(name).resetRelativeEncoder();
    m_updateMutex.unlock();
}

bool Mouse::hasSensor(const std::string& name) const {
    return ContainerUtilities::mapContains(m_sensors, name);
}

double Mouse::readSensor(const std::string& name) const {
    SIM_ASSERT_TR(hasSensor(name));
    return m_sensors.at(name).read();
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

std::pair<Cartesian, Radians> Mouse::getCurrentSensorPositionAndDirection(
        const Sensor& sensor,
        const Cartesian& currentTranslation,
        const Radians& currentRotation) const {
    Cartesian translationDelta = currentTranslation - getInitialTranslation();
    Radians rotationDelta = currentRotation - getInitialRotation();
    return std::make_pair(
        GeometryUtilities::rotateVertexAroundPoint(
            GeometryUtilities::translateVertex(
                sensor.getInitialPosition(),
                translationDelta),
            rotationDelta,
            currentTranslation),
        sensor.getInitialDirection() + rotationDelta
    );
}

void Mouse::setWheelSpeedsForMovement(double fractionOfMaxSpeed, double forwardFactor, double turnFactor) {

    // We can think about setting the wheels speeds for particular movements as
    // a linear combination of the forward movement and the turn movement. For
    // instance, the (normalized) linear combination of the forward and turn
    // components for moving forward is just 1.0 and 0.0, respectively. For
    // turning left, it's 0.0 and 1.0, respectively, and for turning right it's
    // 0.0 and -1.0, respectively. For curve turns, it's some other linear
    // combination. Note that we normalize here since we don't know anything
    // about the wheel speeds for a particular component. Thus, we must ensure
    // that the sum of the magnitudes of the components is in [0.0, 1.0] so
    // that we don't try to set any wheel speeds greater than the max.

    // First we normalize the factors so that the sum of the magnitudes is in [0.0, 1.0]
    double factorMagnitude = std::abs(forwardFactor) + std::abs(turnFactor);
    double normalizedForwardFactor = forwardFactor / factorMagnitude;
    double normalizedTurnFactor = turnFactor / factorMagnitude;

    // Now we just double check that the magnitudes are where we expect them to be
    double normalizedFactorMagnitude = std::abs(normalizedForwardFactor) + std::abs(normalizedTurnFactor);
    SIM_ASSERT_LE(0.0, normalizedFactorMagnitude);
    SIM_ASSERT_LE(normalizedFactorMagnitude, 1.0);

    // Now set the wheel speeds based on the normalized factors
    std::map<std::string, RadiansPerSecond> wheelSpeeds;
    for (const std::pair<const std::string&, const Wheel&>& wheel : m_wheels) {
        SIM_ASSERT_TR(ContainerUtilities::mapContains(m_wheelSpeedAdjustmentFactors, wheel.first));
        std::pair<double, double> adjustmentFactors = m_wheelSpeedAdjustmentFactors.at(wheel.first);
        wheelSpeeds.insert(
            std::make_pair(
                wheel.first,
                (
                    wheel.second.getMaxAngularVelocityMagnitude() *
                    fractionOfMaxSpeed *
                    (
                        normalizedForwardFactor * adjustmentFactors.first +
                        normalizedTurnFactor * adjustmentFactors.second
                    )
                )
            )
        );
    }
    setWheelSpeeds(wheelSpeeds);
}

std::map<std::string, WheelEffect> Mouse::getWheelEffects(
        const Cartesian& initialTranslation,
        const Radians& initialRotation,
        const std::map<std::string, Wheel>& wheels) const {

    std::map<std::string, WheelEffect> wheelEffects;

    for (const std::pair<const std::string&, const Wheel&>& wheel : wheels) {
        wheelEffects.insert(
            std::make_pair(
                wheel.first,
                WheelEffect(
                    initialTranslation,
                    initialRotation,
                    wheel.second
                )
            )
        );
    }

    return wheelEffects;
}

std::map<std::string, std::pair<double, double>> Mouse::getWheelSpeedAdjustmentFactors(
        const std::map<std::string, Wheel>& wheels,
        const std::map<std::string, WheelEffect>& wheelEffects) const {

    // Right now, the heueristic that we're using is that if a wheel greatly
    // contributes to moving forward or turning, then its adjustment factors
    // should be high for moving forward or turning, respectively. That is, if
    // we've got a wheel that's facing to the right, we don't want to turn that
    // wheel when we're trying to move forward. Instead, we should only turn
    // the wheels that will actually contribute to the forward movement of the
    // mouse. I'm not yet sure if we should take wheel size and/or max angular
    // velocity magnitude into account, but I've done so here.

    // First, construct the rates of change pairs
    std::map<std::string, std::pair<MetersPerSecond, RadiansPerSecond>> ratesOfChangePairs;
    for (const std::pair<const std::string&, const WheelEffect&>& pair : wheelEffects) {
        std::tuple<MetersPerSecond, MetersPerSecond, RadiansPerSecond> effects =
            pair.second.getEffects(m_wheels.at(pair.first).getMaxAngularVelocityMagnitude());
        ratesOfChangePairs.insert(
            std::make_pair(
                pair.first,
                std::make_pair(
                    std::get<0>(effects),
                    std::get<2>(effects)
                )
            )
        );
    }

    // Then determine the largest magnitude
    MetersPerSecond maxForwardRateOfChangeMagnitude(0);
    RadiansPerSecond maxRadialRateOfChangeMagnitude(0);
    for (const std::pair<std::string, std::pair<MetersPerSecond, RadiansPerSecond>>& pair : ratesOfChangePairs) {
        MetersPerSecond forwardRateOfChangeMagnitude(std::abs(pair.second.first.getMetersPerSecond()));
        RadiansPerSecond radialRateOfChangeMagnitude(std::abs(pair.second.second.getRadiansPerSecond()));
        if (maxForwardRateOfChangeMagnitude < forwardRateOfChangeMagnitude) {
            maxForwardRateOfChangeMagnitude = forwardRateOfChangeMagnitude;
        }
        if (maxRadialRateOfChangeMagnitude < radialRateOfChangeMagnitude) {
            maxRadialRateOfChangeMagnitude = radialRateOfChangeMagnitude;
        }
    }

    // Then divide by the largest magnitude, ensuring values in [-1.0, 1.0]
    std::map<std::string, std::pair<double, double>> adjustmentFactors;
    for (std::pair<std::string, std::pair<MetersPerSecond, RadiansPerSecond>> pair : ratesOfChangePairs) {
        double normalizedForwardContribution = pair.second.first / maxForwardRateOfChangeMagnitude;
        double normalizedRadialContribution = pair.second.second / maxRadialRateOfChangeMagnitude;
        SIM_ASSERT_LE(-1.0, normalizedForwardContribution);
        SIM_ASSERT_LE(-1.0, normalizedRadialContribution);
        SIM_ASSERT_LE(normalizedForwardContribution, 1.0);
        SIM_ASSERT_LE(normalizedRadialContribution, 1.0);
        adjustmentFactors.insert(
            std::make_pair(
                pair.first,
                std::make_pair(
                    normalizedForwardContribution,
                    normalizedRadialContribution)));
    }
    
    return adjustmentFactors;
}

} // namespace sim

#include "Mouse.h"

#include <QMutableMapIterator>
#include <QPair>
#include <QVector>
#include <QtMath>

#include "units/Distance.h"
#include "units/Speed.h"

#include "Assert.h"
#include "GeometryUtilities.h"
#include "MouseParser.h"
#include "Param.h"

namespace mms {

Mouse::Mouse(const Maze* maze) :
    m_maze(maze),
    m_crashed(false) {

    // The initial translation of the mouse is just the center of the starting tile
    Distance halfOfTileDistance = Distance::Meters((P()->wallLength() + P()->wallWidth()) / 2.0);
    m_initialTranslation = Coordinate::Cartesian(halfOfTileDistance, halfOfTileDistance);
    m_currentTranslation = m_initialTranslation;

    // The initial rotation of the mouse is determined by the starting tile walls
    Direction optimalStartingDirection = maze->getOptimalStartingDirection();
    m_startedDirection = optimalStartingDirection;
    m_startingDirection = m_startedDirection;
    m_initialRotation = DIRECTION_TO_ANGLE().value(m_startingDirection);
    m_currentRotation = m_initialRotation;
}

bool Mouse::reload(const QString& mouseFile) {

    // We begin with the assumption that the initialization will succeed
    bool success = true;

    // Create the mouse parser object
    MouseParser parser(mouseFile, &success);
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
    QVector<Polygon> polygons;
    polygons.push_back(m_initialBodyPolygon);
    for (const Wheel& wheel : m_wheels) {
        polygons.push_back(wheel.getInitialPolygon());
    }
    for (const Sensor& sensor : m_sensors) {
        polygons.push_back(sensor.getInitialPolygon());
    }
    m_initialCollisionPolygon = GeometryUtilities::convexHull(polygons);

    // Initialize the center of mass polygon
    m_initialCenterOfMassPolygon = GeometryUtilities::createCirclePolygon(
        m_initialTranslation,
        Distance::Meters(.005),
        8 // Num sides
    );

    // Force triangulation of the drawable polygons, thus ensuring
    // that we only triangulate once, at the beginning of execution
    m_initialBodyPolygon.getTriangles();
    m_initialCollisionPolygon.getTriangles();
    m_initialCenterOfMassPolygon.getTriangles();
    for (const Wheel& wheel : m_wheels) {
        wheel.getInitialPolygon().getTriangles();
    }
    for (const Sensor& sensor : m_sensors) {
        sensor.getInitialPolygon().getTriangles();
        sensor.getInitialViewPolygon().getTriangles();
    }

    // Lastly, keep track of the mouse file we just successfully loaded
    m_mouseFile = mouseFile;

    // Return success
    return success;
}

QString Mouse::getMouseFile() const {
    return m_mouseFile;
}

bool Mouse::didCrash() const {
    return m_crashed;
}

void Mouse::setCrashed() {
    m_crashed = true;
}

void Mouse::reset() {
    teleport(
        getInitialTranslation(),
        DIRECTION_TO_ANGLE().value(m_startingDirection)
    );
    m_startedDirection = m_startingDirection;
    m_crashed = false;
}

void Mouse::teleport(const Coordinate& translation, const Angle& rotation) {
    m_currentTranslation = translation;
    m_currentRotation = rotation;
}

Direction Mouse::getStartedDirection() const {
    return m_startedDirection;
}

void Mouse::setStartingDirection(Direction direction) {
    m_startingDirection = direction;
}

Coordinate Mouse::getInitialTranslation() const {
    return m_initialTranslation;
}

Coordinate Mouse::getCurrentTranslation() const {
    return m_currentTranslation;
}

Angle Mouse::getCurrentRotation() const {
    return m_currentRotation;
}

QPair<int, int> Mouse::getCurrentDiscretizedTranslation() const {
    static Distance tileLength = Distance::Meters(P()->wallLength() + P()->wallWidth());
    Coordinate currentTranslation = getCurrentTranslation();
    int x = static_cast<int>(qFloor(currentTranslation.getX() / tileLength));
    int y = static_cast<int>(qFloor(currentTranslation.getY() / tileLength));
    return {x, y};
}

Direction Mouse::getCurrentDiscretizedRotation() const {
    int dir = static_cast<int>(qFloor(
        (getCurrentRotation() + Angle::Degrees(45)).getRadiansZeroTo2pi() /
        Angle::Degrees(90).getRadiansZeroTo2pi()
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
        default:
            ASSERT_NEVER_RUNS();
    }
}

Polygon Mouse::getCurrentBodyPolygon(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const {
    return getCurrentPolygon(
        m_initialBodyPolygon,
        currentTranslation,
        currentRotation);
}

Polygon Mouse::getCurrentCollisionPolygon(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const {
    return getCurrentPolygon(
        m_initialCollisionPolygon,
        currentTranslation,
        currentRotation);
}

Polygon Mouse::getCurrentCenterOfMassPolygon(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const {
    return getCurrentPolygon(
        m_initialCenterOfMassPolygon,
        currentTranslation,
        currentRotation);
}

QVector<Polygon> Mouse::getCurrentWheelPolygons(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const {
    QVector<Polygon> polygons;
    // TODO: upforgrabs
    // All getPolygons functions require a lock to deal with the implicit
    // sharing problem:
    // http://doc.qt.io/qt-5/containers.html#implicit-sharing-iterator-problem.
    // Ideally, this shouldn't be the case (we should be able to perform reads
    // without having to worry about writes). Your task is to figure out how to
    // remove the lock without causing segfaults.
    m_mutex.lock();
    for (const Wheel& wheel : m_wheels.values()) {
        polygons.push_back(
            getCurrentPolygon(
                wheel.getInitialPolygon(),
                currentTranslation,
                currentRotation));
    }
    m_mutex.unlock();
    return polygons;
}

QVector<Polygon> Mouse::getCurrentSensorPolygons(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const {
    QVector<Polygon> polygons;
    m_mutex.lock();
    for (const Sensor& sensor : m_sensors.values()) {
        polygons.push_back(
            getCurrentPolygon(
                sensor.getInitialPolygon(),
                currentTranslation,
                currentRotation));
    }
    m_mutex.unlock();
    return polygons;
}

QVector<Polygon> Mouse::getCurrentSensorViewPolygons(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const {
    QVector<Polygon> polygons;
    m_mutex.lock();
    for (const Sensor& sensor : m_sensors.values()) {
        QPair<Coordinate, Angle> translationAndRotation =
            getCurrentSensorPositionAndDirection(
                sensor,
                currentTranslation,
                currentRotation);
        polygons.push_back(
            sensor.getCurrentViewPolygon(
                translationAndRotation.first,
                translationAndRotation.second,
                *m_maze));
    }
    m_mutex.unlock();
    return polygons;
}

void Mouse::update(const Duration& elapsed) {

    // NOTE: This is a *very* performance critical function

    if (m_crashed) {
        return;
    }

    m_mutex.lock();

    Speed sumDx;
    Speed sumDy;
    AngularVelocity sumDr;

    // Iterate over all of the wheels
    QMutableMapIterator<QString, Wheel> wheelIterator(m_wheels);
    while (wheelIterator.hasNext()) {

        // TODO: MACK - this segfaults :/
        auto pair = wheelIterator.next();

        // Update the rotation of the wheel
        pair.value().updateRotation(pair.value().getAngularVelocity() * elapsed);

        // Get the effects on the rate of change of translation, both forward
        // and sideways, and rotation of the mouse due to this particular wheel
        std::tuple<Speed, Speed, AngularVelocity> effects =
            m_wheelEffects.value(pair.key()).getEffects(pair.value().getAngularVelocity());

        // The effect of the forward component
        sumDx += std::get<0>(effects) * getCurrentRotation().getCos();
        sumDy += std::get<0>(effects) * getCurrentRotation().getSin();

        // The effect of the sideways component
        sumDx += std::get<1>(effects) * getCurrentRotation().getSin();
        sumDy += std::get<1>(effects) * getCurrentRotation().getCos() * -1;

        // The effect of the rotation component
        sumDr += std::get<2>(effects);
    }

    Speed aveDx = sumDx / static_cast<double>(m_wheels.size());
    Speed aveDy = sumDy / static_cast<double>(m_wheels.size());
    AngularVelocity aveDr = sumDr / static_cast<double>(m_wheels.size());

    m_currentGyro = aveDr;
    m_currentRotation += aveDr * elapsed;
    m_currentTranslation += Coordinate::Cartesian(aveDx * elapsed, aveDy * elapsed);

    // Update all of the sensor readings
    QMutableMapIterator<QString, Sensor> sensorIterator(m_sensors);
    while (sensorIterator.hasNext()) {
        auto pair = sensorIterator.next();
        QPair<Coordinate, Angle> translationAndRotation =
            getCurrentSensorPositionAndDirection(
                pair.value(),
                m_currentTranslation,
                m_currentRotation);
        pair.value().updateReading(
            translationAndRotation.first,
            translationAndRotation.second,
            *m_maze);
    }

    m_mutex.unlock();
}

bool Mouse::hasWheel(const QString& name) const {
    return m_wheels.contains(name);
}

AngularVelocity Mouse::getWheelMaxSpeed(const QString& name) const {
    ASSERT_TR(m_wheels.contains(name));
    return m_wheels.value(name).getMaxAngularVelocityMagnitude();
}

void Mouse::setWheelSpeeds(const QMap<QString, AngularVelocity>& wheelSpeeds) {
    m_mutex.lock();
    for (const auto& pair : ContainerUtilities::items(wheelSpeeds)) {
        ASSERT_TR(m_wheels.contains(pair.first));
        ASSERT_LE(
            std::abs(pair.second.getRevolutionsPerMinute()),
            getWheelMaxSpeed(pair.first).getRevolutionsPerMinute());
        m_wheels[pair.first].setAngularVelocity(pair.second);
    }
    m_mutex.unlock();
}

void Mouse::setWheelSpeedsForMoveForward(double fractionOfMaxSpeed) {
    setWheelSpeedsForMovement(fractionOfMaxSpeed, 1.0, 0.0);
}

void Mouse::setWheelSpeedsForCurveLeft(double fractionOfMaxSpeed, const Distance& radius) {
    QPair<double, double> curveTurnFactors =
        m_curveTurnFactorCalculator.getCurveTurnFactors(radius);
    setWheelSpeedsForMovement(
        fractionOfMaxSpeed,
        curveTurnFactors.first,
        curveTurnFactors.second);
}

void Mouse::setWheelSpeedsForCurveRight(double fractionOfMaxSpeed, const Distance& radius) {
    QPair<double, double> curveTurnFactors =
        m_curveTurnFactorCalculator.getCurveTurnFactors(radius);
    setWheelSpeedsForMovement(
        fractionOfMaxSpeed,
        curveTurnFactors.first,
        -1 * curveTurnFactors.second);
}

void Mouse::stopAllWheels() {
    QMap<QString, AngularVelocity> wheelSpeeds;
    for (const QString& name : m_wheels.keys()) {
        wheelSpeeds.insert(name, AngularVelocity::RadiansPerSecond(0));
    }
    setWheelSpeeds(wheelSpeeds);
}

EncoderType Mouse::getWheelEncoderType(const QString& name) const {
    ASSERT_TR(hasWheel(name));
    return m_wheels.value(name).getEncoderType();
}

double Mouse::getWheelEncoderTicksPerRevolution(const QString& name) const {
    ASSERT_TR(hasWheel(name));
    return m_wheels.value(name).getEncoderTicksPerRevolution();
}

int Mouse::readWheelAbsoluteEncoder(const QString& name) const {
    ASSERT_TR(hasWheel(name));
    m_mutex.lock();
    int encoderReading = m_wheels.value(name).readAbsoluteEncoder();
    m_mutex.unlock();
    return encoderReading;
}

int Mouse::readWheelRelativeEncoder(const QString& name) const {
    ASSERT_TR(hasWheel(name));
    m_mutex.lock();
    int encoderReading = m_wheels.value(name).readRelativeEncoder();
    m_mutex.unlock();
    return encoderReading;
}

void Mouse::resetWheelRelativeEncoder(const QString& name) {
    ASSERT_TR(hasWheel(name));
    m_mutex.lock();
    // TODO: upforgrabs
    // Use value() here instead of [] notation
    m_wheels[name].resetRelativeEncoder();
    m_mutex.unlock();
}

bool Mouse::hasSensor(const QString& name) const {
    return m_sensors.contains(name);
}

double Mouse::readSensor(const QString& name) const {
    ASSERT_TR(hasSensor(name));
    return m_sensors.value(name).read();
}

AngularVelocity Mouse::readGyro() const {
    return m_currentGyro;
}

Polygon Mouse::getCurrentPolygon(
        const Polygon& initialPolygon,
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const {
    return initialPolygon
        .translate(currentTranslation - getInitialTranslation())
        .rotateAroundPoint(currentRotation - m_initialRotation, currentTranslation);
}

QPair<Coordinate, Angle> Mouse::getCurrentSensorPositionAndDirection(
        const Sensor& sensor,
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const {
    Coordinate translationDelta = currentTranslation - getInitialTranslation();
    Angle rotationDelta = currentRotation - m_initialRotation;
    return {
        GeometryUtilities::rotateVertexAroundPoint(
            GeometryUtilities::translateVertex(
                sensor.getInitialPosition(),
                translationDelta),
            rotationDelta,
            currentTranslation),
        sensor.getInitialDirection() + rotationDelta
    };
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
    ASSERT_LE(0.0, normalizedFactorMagnitude);
    ASSERT_LE(normalizedFactorMagnitude, 1.0);

    // Now set the wheel speeds based on the normalized factors
    QMap<QString, AngularVelocity> wheelSpeeds;
    for (const auto& pair : ContainerUtilities::items(m_wheels)) {
        ASSERT_TR(m_wheelSpeedAdjustmentFactors.contains(pair.first));
        QPair<double, double> adjustmentFactors = m_wheelSpeedAdjustmentFactors.value(pair.first);
        wheelSpeeds.insert(
            pair.first,
            (
                pair.second.getMaxAngularVelocityMagnitude() *
                fractionOfMaxSpeed *
                (
                    normalizedForwardFactor * adjustmentFactors.first +
                    normalizedTurnFactor * adjustmentFactors.second
                )
            )
        );
    }
    setWheelSpeeds(wheelSpeeds);
}

QMap<QString, WheelEffect> Mouse::getWheelEffects(
        const Coordinate& initialTranslation,
        const Angle& initialRotation,
        const QMap<QString, Wheel>& wheels) const {
    QMap<QString, WheelEffect> wheelEffects;
    for (const auto& pair : ContainerUtilities::items(wheels)) {
        wheelEffects.insert(
            pair.first,
            WheelEffect(
                initialTranslation,
                initialRotation,
                pair.second
            )
        );
    }
    return wheelEffects;
}

QMap<QString, QPair<double, double>> Mouse::getWheelSpeedAdjustmentFactors(
        const QMap<QString, Wheel>& wheels,
        const QMap<QString, WheelEffect>& wheelEffects) const {

    // Right now, the heueristic that we're using is that if a wheel greatly
    // contributes to moving forward or turning, then its adjustment factors
    // should be high for moving forward or turning, respectively. That is, if
    // we've got a wheel that's facing to the right, we don't want to turn that
    // wheel when we're trying to move forward. Instead, we should only turn
    // the wheels that will actually contribute to the forward movement of the
    // mouse. I'm not yet sure if we should take wheel size and/or max angular
    // velocity magnitude into account, but I've done so here.

    // First, construct the rates of change pairs
    QMap<QString, QPair<Speed, AngularVelocity>> ratesOfChangePairs;
    for (const auto& pair : ContainerUtilities::items(wheelEffects)) {
        std::tuple<Speed, Speed, AngularVelocity> effects =
            pair.second.getEffects(wheels.value(pair.first).getMaxAngularVelocityMagnitude());
        ratesOfChangePairs.insert(
            pair.first,
            {
                std::get<0>(effects),
                std::get<2>(effects)
            }
        );
    }

    // Then determine the largest magnitude
    Speed maxForwardRateOfChangeMagnitude;
    AngularVelocity maxRadialRateOfChangeMagnitude;
    for (const auto& pair : ContainerUtilities::items(ratesOfChangePairs)) {
        Speed forwardRateOfChangeMagnitude = Speed::MetersPerSecond(
            std::abs(pair.second.first.getMetersPerSecond()));
        AngularVelocity radialRateOfChangeMagnitude = AngularVelocity::RadiansPerSecond(
            std::abs(pair.second.second.getRadiansPerSecond()));
        if (maxForwardRateOfChangeMagnitude < forwardRateOfChangeMagnitude) {
            maxForwardRateOfChangeMagnitude = forwardRateOfChangeMagnitude;
        }
        if (maxRadialRateOfChangeMagnitude < radialRateOfChangeMagnitude) {
            maxRadialRateOfChangeMagnitude = radialRateOfChangeMagnitude;
        }
    }

    // Then divide by the largest magnitude, ensuring values in [-1.0, 1.0]
    QMap<QString, QPair<double, double>> adjustmentFactors;
    for (const auto& pair : ContainerUtilities::items(ratesOfChangePairs)) {
        double normalizedForwardContribution = pair.second.first / maxForwardRateOfChangeMagnitude;
        double normalizedRadialContribution = (
            pair.second.second.getRadiansPerSecond() /
            maxRadialRateOfChangeMagnitude.getRadiansPerSecond()
        );
        ASSERT_LE(-1.0, normalizedForwardContribution);
        ASSERT_LE(-1.0, normalizedRadialContribution);
        ASSERT_LE(normalizedForwardContribution, 1.0);
        ASSERT_LE(normalizedRadialContribution, 1.0);
        adjustmentFactors.insert(
            pair.first,
            {
                normalizedForwardContribution,
                normalizedRadialContribution
            }
        );
    }
    
    return adjustmentFactors;
}

} // namespace mms

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
    m_translation = m_initialTranslation; // TODO: add a parameter for starting postiion offset in the x and y directions

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

    // Return success
    return true;
}

Polygon Mouse::getCollisionPolygon() const {
    return m_initialCollisionPolygon.translate(m_translation - m_initialTranslation).rotateAroundPoint(m_rotation, m_translation);
}

Polygon Mouse::getBodyPolygon() const {
    return m_initialBodyPolygon.translate(m_translation - m_initialTranslation).rotateAroundPoint(m_rotation, m_translation);
}

std::vector<Polygon> Mouse::getWheelPolygons() const {

    // TODO: MACK - dedup with the below getSensorPolygons function

    // Get the initial wheel polygons
    std::vector<Polygon> initialPolygons;
    for (std::pair<std::string, Wheel> pair : m_wheels) {
        initialPolygons.push_back(pair.second.getInitialPolygon());
    }

    // Translate and rotate the polygons appropriately
    std::vector<Polygon> adjustedPolygons;
    for (Polygon polygon : initialPolygons) {
        adjustedPolygons.push_back(
            polygon.translate(m_translation - m_initialTranslation).rotateAroundPoint(m_rotation, m_translation));
    }

    return adjustedPolygons;
}

std::vector<Polygon> Mouse::getSensorPolygons() const {

    // Get the initial sensor polygons
    std::vector<Polygon> initialPolygons;
    for (std::pair<std::string, Sensor> pair : m_sensors) {
        initialPolygons.push_back(pair.second.getInitialPolygon());
    }

    // Translate and rotate the polygons appropriately
    std::vector<Polygon> adjustedPolygons;
    for (Polygon polygon : initialPolygons) {
        adjustedPolygons.push_back(
            polygon.translate(m_translation - m_initialTranslation).rotateAroundPoint(m_rotation, m_translation));
    }

    return adjustedPolygons;
}

std::vector<Polygon> Mouse::getViewPolygons() const {

    // Get the current view for the all of the sensors
    std::vector<Polygon> polygons;
    for (std::pair<std::string, Sensor> pair : m_sensors) {
        Polygon adjusted = pair.second.getInitialView().translate(m_translation - m_initialTranslation)
            .rotateAroundPoint(m_rotation, m_translation);
        polygons.push_back(pair.second.getCurrentView(
            adjusted.getVertices().at(0), m_rotation + pair.second.getInitialDirection(), *m_maze));
    }
    return polygons;
}

void Mouse::update(const Duration& elapsed) {

    /*
     *  In a differential drive system (two-wheeled drive system), the angular velocities of
     *  each of the two wheels completely determine the movement of the robot. The equations
     *  for the instantaneous change in rotation and translation (with respect to the robot)
     *  are as follows:
     *
     *      dx/dt = (rightWheelSpeed - leftWheelSpeed) / 2
     *      dy/dt = 0
     *      d0/dt = (rightWheelSpeed + leftWheelSpeed) / base
     *
     *  where the coordinate axes with respect to the robot are as follows:
     *
     *               x
     *               ^
     *               |
     *               |
     *              / \
     *      y <----0---0
     *
     *  Note that dy/dt = 0 since it's impossible for the robot to move laterally. Also note
     *  that since the left and right wheels are oriented oppositely, a positive wheel speed
     *  of the right wheel means that the wheel moves in the negative y direction (with
     *  respect to the robot) while a positive wheel speed of the left wheel means that the
     *  wheel moves in the positive y direction (again, with respect to the robot). Given
     *  these few equations, we can easily approximate the motion of the robot with respect
     *  to the maze by multiplying the instantaneous rate of change in the translation and
     *  rotation with the elapsed time. This is certainly an approximation because the rotation
     *  of the robot is not constant throughout the duration of the motion. Thus, while the
     *  rate of change of rotation is not a function of time, the rate of change of the
     *  translation of robot (with respect the the maze) is a function of time. While there
     *  is a closed form solution for the translation of the robot given a non-zero rate of
     *  rotation of the robot, it's unnecessary to use it here. Our elapsed times should be
     *  small and thus the change in rotation should be mostly negligible.
     */

    // TODO: MACK - update this to take wheel direction into account

    // First get the speed of each wheel (atomically)
    m_wheelMutex.lock();
    // TODO: MACK - we'll have to change this for arbitrarily many wheels
    MetersPerSecond rightWheelSpeed(
        m_wheels.at("right").getAngularVelocity().getRadiansPerSecond() * m_wheels.at("right").getDiameter().getMeters() / 2.0);
    MetersPerSecond leftWheelSpeed(
        m_wheels.at("left").getAngularVelocity().getRadiansPerSecond() * m_wheels.at("left").getDiameter().getMeters() / 2.0);
    m_wheelMutex.unlock();

    // Then get the distance between the two wheels
    Meters base(m_wheels.at("right").getInitialPosition().getX() - m_wheels.at("left").getInitialPosition().getX());

    // Update the amount each wheel has rotated
    // TODO: MACK

    // Update the gyro; -1 since positive wheel angular velocity causes negative gyro angular velocity
    m_gyro = RadiansPerSecond(-1 * (leftWheelSpeed + rightWheelSpeed).getMetersPerSecond() / base.getMeters());
    
    // Update the rotation
    m_rotation += m_gyro * elapsed;

    // Update the translation
    Meters distance((rightWheelSpeed - leftWheelSpeed).getMetersPerSecond() / 2.0 * elapsed.getSeconds());
    m_translation += Polar(distance, Degrees(90) + m_rotation); // This could be optimized
    // TODO: MACK ^^^ The above hardcoded should not be...

    // -----------------------------------------------------------------------------------------------------

    // TODO: This is the technically corect implementation...
    /*
    double BASELINE(m_wheels.at("right").getInitialPosition().getX().getMeters() - m_wheels.at("left").getInitialPosition().getX().getMeters());

    if (fabs(rightWheelSpeed.getMetersPerSecond() == -1*leftWheelSpeed.getMetersPerSecond())) {
        Meters distance((-0.5 * leftWheelSpeed.getMetersPerSecond() + 0.5 * rightWheelSpeed.getMetersPerSecond()) * elapsed.getSeconds());
        m_translation = m_translation + Polar(distance, Radians(M_PI / 2.0) + m_rotation);
        static int i = 0;
        std::cout << i++ << std::endl;
    }
    else {
        leftWheelSpeed = MetersPerSecond(-1*leftWheelSpeed.getMetersPerSecond());

        double x_0 = m_translation.getY().getMeters();
        double y_0 = -1*m_translation.getX().getMeters();

        double x = x_0 +

                  (BASELINE*(rightWheelSpeed+leftWheelSpeed).getMetersPerSecond())
                  /(2.0*(rightWheelSpeed-leftWheelSpeed).getMetersPerSecond())

                  *(sin((rightWheelSpeed-leftWheelSpeed).getMetersPerSecond() * elapsed.getSeconds() / BASELINE + m_rotation.getRadians())
                   -sin(m_rotation.getRadians()));

        double y = y_0 -

                  (BASELINE*(rightWheelSpeed+leftWheelSpeed).getMetersPerSecond())
                  /(2.0*(rightWheelSpeed-leftWheelSpeed).getMetersPerSecond())

                  *(cos((rightWheelSpeed-leftWheelSpeed).getMetersPerSecond() * elapsed.getSeconds() / BASELINE + m_rotation.getRadians())
                   -cos(m_rotation.getRadians()));

        m_translation = Cartesian(Meters(-y), Meters(x));

        leftWheelSpeed = MetersPerSecond(-1*leftWheelSpeed.getMetersPerSecond());
    }

    Radians theta((rightWheelSpeed.getMetersPerSecond() - (-leftWheelSpeed.getMetersPerSecond())) / BASELINE * elapsed.getSeconds());
    m_rotation = m_rotation + theta;
    */
}

void Mouse::setWheelSpeeds(const AngularVelocity& leftWheelSpeed, const AngularVelocity& rightWheelSpeed) {
    m_wheelMutex.lock();
    m_wheels.at("left").setAngularVelocity(leftWheelSpeed);
    m_wheels.at("right").setAngularVelocity(rightWheelSpeed);
    m_wheelMutex.unlock();
}

bool Mouse::hasSensor(const std::string& name) const {
    return SimUtilities::mapContains(m_sensors, name);
}

double Mouse::read(const std::string& name) const {

    // Validate the input
    ASSERT_TR(hasSensor(name));
    Sensor sensor = m_sensors.at(name);

    // Retrieve the current translation and rotation
    Cartesian currentTranslation = m_translation;
    Radians currentRotation = m_rotation;

    // Determine the reading
    Polygon fullView = sensor.getInitialView().translate(currentTranslation - m_initialTranslation)
        .rotateAroundPoint(currentRotation, currentTranslation);
    Polygon currentView = sensor.getCurrentView(
        fullView.getVertices().at(0), currentRotation + sensor.getInitialDirection(), *m_maze);
    return 1.0 - currentView.area().getMetersSquared() / fullView.area().getMetersSquared();
}

Seconds Mouse::getReadDuration(const std::string& name) const {
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

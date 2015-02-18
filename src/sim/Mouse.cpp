#include "Mouse.h"

#include <cmath>
#include <iostream>

#include "Param.h"
#include "SimUtilities.h"
#include "units/MetersPerSecond.h"
#include "units/Polar.h"

namespace sim {

Mouse::Mouse() : m_translation(Cartesian(0, 0)), m_rotation(Radians(0)), parser("src/sim/mouse.xml") {

    // TODO: Validate the contents of the mouse file (like valid mouse starting position)
    // TODO: Right now, the size of the mouse is dependent on the size of the maze- we should fix this...
    // TODO: SOM
    Meters radius = parser.getWheelMeas("LeftWheel", "radius");
    Meters width = parser.getWheelMeas("LeftWheel", "width");
    Cartesian pos = parser.getWheelPosition("LeftWheel");

    m_leftWheel = Wheel(radius, width, pos);

    radius = parser.getWheelMeas("RightWheel", "radius");
    width = parser.getWheelMeas("RightWheel", "width");
    pos = parser.getWheelPosition("RightWheel");

    m_rightWheel = Wheel(radius, width, pos);

    // Create the vertices for the mouse
    std::vector<Cartesian> vertices = parser.getBody();
    m_body.setVertices(vertices);
}

// TODO: Shouldn't need this method
Cartesian Mouse::getTranslation() const {

    Cartesian centerOfMass((m_rightWheel.getPosition().getX() + m_leftWheel.getPosition().getX())/2.0, m_rightWheel.getPosition().getY());
    Polar vec(centerOfMass.getRho(), m_rotation + centerOfMass.getTheta()); // The current rotation vector
    Cartesian disp(vec.getX() - centerOfMass.getX(), vec.getY() - centerOfMass.getY());

    return Cartesian(m_translation.getX() - disp.getX(), m_translation.getY() - disp.getY());
}

// TODO: Better interface for polygons
std::vector<Polygon> Mouse::getShapes() const {

    // Create the shapes vector
    std::vector<Polygon> shapes;

    // The mouse body
    shapes.push_back(m_body);

    // TODO: We really only need to do this logic once
    // TODO: This is *really* ugly... fix this...
    // Right wheel
    std::vector<Cartesian> rightWheelPolygon;

    Meters rightWheelHalfWidth = m_rightWheel.getWidth() / 2.0;
    Meters rightWheelRadius = m_rightWheel.getRadius();
    Cartesian rightWheelPosition = m_rightWheel.getPosition();

    rightWheelPolygon.push_back(rightWheelPosition + Cartesian(
        Meters((rightWheelHalfWidth * -1).getMeters()), Meters((rightWheelRadius * -1).getMeters())));
    rightWheelPolygon.push_back(rightWheelPosition + Cartesian(
        Meters((rightWheelHalfWidth * -1).getMeters()), Meters((rightWheelRadius *  1).getMeters())));
    rightWheelPolygon.push_back(rightWheelPosition + Cartesian(
        Meters((rightWheelHalfWidth *  1).getMeters()), Meters((rightWheelRadius *  1).getMeters())));
    rightWheelPolygon.push_back(rightWheelPosition + Cartesian(
        Meters((rightWheelHalfWidth *  1).getMeters()), Meters((rightWheelRadius * -1).getMeters())));

    shapes.push_back(Polygon(rightWheelPolygon));

    // Left wheel
    std::vector<Cartesian> leftWheelPolygon;

    Meters leftWheelHalfWidth = m_leftWheel.getWidth() / 2.0;
    Meters leftWheelRadius = m_leftWheel.getRadius();
    Cartesian leftWheelPosition = m_leftWheel.getPosition();

    leftWheelPolygon.push_back(leftWheelPosition + Cartesian(
        Meters((leftWheelHalfWidth * -1).getMeters()), Meters((leftWheelRadius * -1).getMeters())));
    leftWheelPolygon.push_back(leftWheelPosition + Cartesian(
        Meters((leftWheelHalfWidth * -1).getMeters()), Meters((leftWheelRadius *  1).getMeters())));
    leftWheelPolygon.push_back(leftWheelPosition + Cartesian(
        Meters((leftWheelHalfWidth *  1).getMeters()), Meters((leftWheelRadius *  1).getMeters())));
    leftWheelPolygon.push_back(leftWheelPosition + Cartesian(
        Meters((leftWheelHalfWidth *  1).getMeters()), Meters((leftWheelRadius * -1).getMeters())));

    shapes.push_back(Polygon(leftWheelPolygon));

    // TODO : Clear this
    std::vector<Polygon> adjustedShapes;
    for (int i = 0; i < shapes.size(); i += 1) {
        adjustedShapes.push_back(shapes.at(i).rotateAroundPoint(m_rotation, Cartesian(Meters(0.0), Meters(0.0))).translate(getTranslation()));
    }
    return adjustedShapes;
}

void Mouse::update(const Time& elapsed) {

    /*
     *  In a differential drive system (two-wheeled drive system), the angular velocities of
     *  each of the two wheels completely determine the movement of the robot. The equations
     *  for the instantaneous change in rotation and translation (with respect to the robot)
     *  are as follows:
     *
     *      dx/dt = 0
     *      dy/dt = (rightWheelSpeed - leftWheelSpeed) / 2
     *      d0/dt = (rightWheelSpeed + leftWheelSpeed) / base
     *
     *  Note that dx/dy = 0 since it's impossible for the robot to move laterally. Also note
     *  that since the left and right wheels are oriented oppositely, a positive wheel speed
     *  of the right wheel means that the wheel moves in the positive y direction (with
     *  respect to the robot) while a positive wheel speed of the left wheel means that the
     *  wheel moves in the negative y directions (again, with respect to the robot). Given
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

    // TODO: Fix the unit classes so I don't have to get the primitive values as often...

    // First get the speed of each wheel
    MetersPerSecond rightWheelSpeed(
        m_rightWheel.getAngularVelocity().getRadiansPerSecond() * m_rightWheel.getRadius().getMeters());
    MetersPerSecond leftWheelSpeed(
        m_leftWheel.getAngularVelocity().getRadiansPerSecond() * m_leftWheel.getRadius().getMeters());

    // Then get the distance between the two wheels
    Meters base(m_rightWheel.getPosition().getX() - m_leftWheel.getPosition().getX());

    // Update the rotation
    m_rotation += Radians((rightWheelSpeed + leftWheelSpeed).getMetersPerSecond() / base.getMeters() * elapsed.getSeconds());

    // Update the translation. Note that we have to add M_PI / 2.0 to the rotation since we measure rotation
    // from the positive x-axis but, by default, the robot has 0 rotation and moves in the positive y direction.
    Meters distance((rightWheelSpeed - leftWheelSpeed).getMetersPerSecond() / 2.0 * elapsed.getSeconds());
    m_translation += Polar(distance, Radians(M_PI / 2.0) + m_rotation);

    // TODO: When we add M_PI/2.0, we assume that all robots will be facing vertically to start
}

// TODO... better interface
Wheel* Mouse::getRightWheel() {
    return &m_rightWheel;
}

Wheel* Mouse::getLeftWheel() {
    return &m_leftWheel;
}

/*
void Mouse::stepBoth() {
    m_leftWheel.rotate(Radians(M_PI/100.0));
    m_rightWheel.rotate(Radians(M_PI/100.0));
}
void Mouse::update() {
    Radians leftRotation = m_leftWheel.popRotation();
    Radians rightRotation = m_rightWheel.popRotation();
}
*/

} // namespace sim

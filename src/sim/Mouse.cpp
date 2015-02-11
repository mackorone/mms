#include "Mouse.h"

#include <cmath>
#include <iostream>

#include "Constants.h"
#include "Utilities.h"

#include "units/MetersPerSecond.h"
#include "units/Polar.h"

namespace sim {

Mouse::Mouse() : m_translation(Cartesian(0, 0)), m_rotation(Radians(0)),
        m_rightWheel(Meters(0.02), Meters(0.025), Cartesian(P()->wallLength() + P()->wallWidth() - 0.06, 0.085)),
        m_leftWheel(Meters(0.02), Meters(0.025), Cartesian(0.06, 0.085)) {
    // Create the vertices for the mouse
    std::vector<Cartesian> vertices;
    vertices.push_back(Cartesian(0.06, 0.06));
    vertices.push_back(Cartesian(0.06, P()->wallLength() + P()->wallWidth() - 0.06));
    vertices.push_back(Cartesian(P()->wallLength() + P()->wallWidth() - 0.06, P()->wallLength() + P()->wallWidth() - 0.06));
    vertices.push_back(Cartesian(P()->wallLength() + P()->wallWidth() - 0.06, 0.06));
    m_body.setVertices(vertices);
}

Cartesian Mouse::getTranslation() const {
    //return m_translation; // TODO: Left off here..., trying to get the in-place rotation to work
    float BASELINE(m_rightWheel.getPosition().getX() - m_leftWheel.getPosition().getX());
    Cartesian WHEEL_CENTER((m_rightWheel.getPosition().getX() + m_leftWheel.getPosition().getX())/2.0, m_rightWheel.getPosition().getY());

    Polar vec(WHEEL_CENTER.getRho(), m_rotation.getRadians() + WHEEL_CENTER.getTheta()); // The current rotation vector
    Cartesian disp(vec.getX() - WHEEL_CENTER.getX(), vec.getY() - WHEEL_CENTER.getY());

    //std::cout << m_rotation.getRadians() << std::endl;
    return Cartesian(m_translation.getX() - disp.getX(), m_translation.getY() - disp.getY());
}

Radians Mouse::getRotation() const {
    return m_rotation;
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
        (rightWheelHalfWidth * -1).getMeters(), (rightWheelRadius * -1).getMeters()));
    rightWheelPolygon.push_back(rightWheelPosition + Cartesian(
        (rightWheelHalfWidth * -1).getMeters(), (rightWheelRadius *  1).getMeters()));
    rightWheelPolygon.push_back(rightWheelPosition + Cartesian(
        (rightWheelHalfWidth *  1).getMeters(), (rightWheelRadius *  1).getMeters()));
    rightWheelPolygon.push_back(rightWheelPosition + Cartesian(
        (rightWheelHalfWidth *  1).getMeters(), (rightWheelRadius * -1).getMeters()));

    Polygon rightPoly;
    rightPoly.setVertices(rightWheelPolygon);
    shapes.push_back(rightPoly);

    // Left wheel
    std::vector<Cartesian> leftWheelPolygon;

    Meters leftWheelHalfWidth = m_leftWheel.getWidth() / 2.0;
    Meters leftWheelRadius = m_leftWheel.getRadius();
    Cartesian leftWheelPosition = m_leftWheel.getPosition();

    leftWheelPolygon.push_back(leftWheelPosition + Cartesian(
        (leftWheelHalfWidth * -1).getMeters(), (leftWheelRadius * -1).getMeters()));
    leftWheelPolygon.push_back(leftWheelPosition + Cartesian(
        (leftWheelHalfWidth * -1).getMeters(), (leftWheelRadius *  1).getMeters()));
    leftWheelPolygon.push_back(leftWheelPosition + Cartesian(
        (leftWheelHalfWidth *  1).getMeters(), (leftWheelRadius *  1).getMeters()));
    leftWheelPolygon.push_back(leftWheelPosition + Cartesian(
        (leftWheelHalfWidth *  1).getMeters(), (leftWheelRadius * -1).getMeters()));

    Polygon leftPoly;
    leftPoly.setVertices(leftWheelPolygon);
    shapes.push_back(leftPoly);

    return shapes;
}

void Mouse::update() {
    // TODO: Note - we're going to have do some weird things with the translation here...
    //m_translation = m_translation + Cartesian(0, 0.00001); // TODO: This is bad...
    //m_rotation = m_rotation + Radians(0.0001);

    // TODO: Time step
    float TIMESTEP(.001);

    // Left wheel
    MetersPerSecond dtl(m_leftWheel.getAngularVelocity().getRadiansPerSecond() * m_leftWheel.getRadius().getMeters());
    
    // Right wheel
    MetersPerSecond dtr(m_rightWheel.getAngularVelocity().getRadiansPerSecond() * m_rightWheel.getRadius().getMeters());

    float BASELINE(m_rightWheel.getPosition().getX() - m_leftWheel.getPosition().getX());
    Radians theta((dtr.getMetersPerSecond() - (-dtl.getMetersPerSecond())) / BASELINE * TIMESTEP);
    m_rotation = m_rotation + theta;

    /*
    Cartesian WHEEL_CENTER(BASELINE, m_rightWheel.getPosition().getY());
    Polar vec(WHEEL_CENTER.getRho(), m_rotation.getRadians() + WHEEL_CENTER.getTheta()); // The current displacement vector

    Cartesian disp(vec.getX() - WHEEL_CENTER.getX(), vec.getY() - WHEEL_CENTER.getY());
    */

    // TODO: Direction...
    Meters distance((-0.5 * dtl.getMetersPerSecond() + 0.5 * dtr.getMetersPerSecond()) * TIMESTEP);
    m_translation = m_translation + Polar(distance.getMeters(), M_PI / 2.0 + m_rotation.getRadians()); // TODO

    //m_translation = Cartesian(m_translation.getX() + disp.getX(), m_translation.getY() + disp.getY());



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

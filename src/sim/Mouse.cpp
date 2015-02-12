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

    // TODO: Read in a mouse file here
    // TODO: Validate the contents of the mouse file (like valid mouse starting position)
    // TODO: Right now, the size of the mouse is dependent on the size of the maze- we should fix this...
    // TODO: SOM

    // Create the vertices for the mouse
    std::vector<Cartesian> vertices;
    vertices.push_back(Cartesian(0.06, 0.06));
    vertices.push_back(Cartesian(0.06, P()->wallLength() + P()->wallWidth() - 0.06));
    vertices.push_back(Cartesian(P()->wallLength() + P()->wallWidth() - 0.06, P()->wallLength() + P()->wallWidth() - 0.06));
    vertices.push_back(Cartesian(P()->wallLength() + P()->wallWidth() - 0.06, 0.06));
    m_body.setVertices(vertices);
}

// TODO: Shouldn't need this method
Cartesian Mouse::getTranslation() const {

    Cartesian centerOfMass((m_rightWheel.getPosition().getX() + m_leftWheel.getPosition().getX())/2.0, m_rightWheel.getPosition().getY());
    Polar vec(centerOfMass.getRho(), m_rotation.getRadians() + centerOfMass.getTheta()); // The current rotation vector
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

    // TODO : Clear this
    std::vector<Polygon> adjustedShapes;
    for (int i = 0; i < shapes.size(); i += 1) {
        adjustedShapes.push_back(shapes.at(i).rotate(m_rotation).translate(getTranslation()));
    }
    return adjustedShapes;
}

void Mouse::update(const Time& elapsed) {

    // The "elapsed" argument signifies how much time has passed since our last update. Thus
    // we should adjust the mouses position so that it's where it would be after moving for
    // the "elapsed" duration.

    // TODO: Document this...

    // Left wheel
    MetersPerSecond dtl(m_leftWheel.getAngularVelocity().getRadiansPerSecond() * m_leftWheel.getRadius().getMeters());
    
    // Right wheel
    MetersPerSecond dtr(m_rightWheel.getAngularVelocity().getRadiansPerSecond() * m_rightWheel.getRadius().getMeters());

    float BASELINE(m_rightWheel.getPosition().getX() - m_leftWheel.getPosition().getX());
    Radians theta((dtr.getMetersPerSecond() - (-dtl.getMetersPerSecond())) / BASELINE * elapsed.getSeconds());
    m_rotation = m_rotation + theta;

    /*
    Cartesian WHEEL_CENTER(BASELINE, m_rightWheel.getPosition().getY());
    Polar vec(WHEEL_CENTER.getRho(), m_rotation.getRadians() + WHEEL_CENTER.getTheta()); // The current displacement vector

    Cartesian disp(vec.getX() - WHEEL_CENTER.getX(), vec.getY() - WHEEL_CENTER.getY());
    */

    // TODO: Direction...
    Meters distance((-0.5 * dtl.getMetersPerSecond() + 0.5 * dtr.getMetersPerSecond()) * elapsed.getSeconds());
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

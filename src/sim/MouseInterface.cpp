#include "MouseInterface.h"

#include <GL/freeglut.h>

#include <Meters.h>
#include <MetersPerSecond.h>
#include <Milliseconds.h>
#include <Seconds.h>

#include "Param.h"
#include "State.h"
#include "SimUtilities.h"

#include <iostream> // TODO


// TODO: Diagonals, more discrete interface methods (look ahead), change color of tile, reset, etc, reduce CPU, etc.


namespace sim {

MouseInterface::MouseInterface(Mouse* mouse) : m_mouse(mouse) {
}

MouseInterface::~MouseInterface() {
}

void MouseInterface::setWheelSpeeds(float leftWheelRadiansPerSecond, float rightWheelRadiansPerSecond) {
    m_mouse->setWheelSpeeds(RadiansPerSecond(leftWheelRadiansPerSecond), RadiansPerSecond(rightWheelRadiansPerSecond));
}

float MouseInterface::read(std::string name) {

    // Start the timer
    double start(sim::getHighResTime());

    // Retrieve the value
    float value = m_mouse->read(name);

    // Stop the timer
    double end(sim::getHighResTime());
    double duration = end - start;

    // Display to the user, if requested
    if (sim::P()->printLateSensorReads() && duration > m_mouse->getReadTime(name).getSeconds()) {
        sim::print(std::string("A sensor read was late by ")
            + std::to_string(duration - m_mouse->getReadTime(name).getSeconds())
            + std::string(" seconds, which is ")
            + std::to_string((duration - m_mouse->getReadTime(name).getSeconds())/(m_mouse->getReadTime(name).getSeconds()) * 100)
            + std::string(" percent late."));
    }

    // Sleep for the read time
    sim::sleep(sim::Seconds(std::max(0.0, 1.0/sim::P()->frameRate() - duration)));

    // Return the value
    return value;
}

void MouseInterface::delay(int milliseconds) {
    sim::sleep(Milliseconds(milliseconds));
}

bool MouseInterface::wallFront() {
    return m_mouse->discretizedIsWall(m_mouse->getDiscretizedRotation());
}

bool MouseInterface::wallRight() {
    switch (m_mouse->getDiscretizedRotation()) {
        case NORTH:
            return m_mouse->discretizedIsWall(EAST);
        case EAST:
            return m_mouse->discretizedIsWall(SOUTH);
        case SOUTH:
            return m_mouse->discretizedIsWall(WEST);
        case WEST:
            return m_mouse->discretizedIsWall(NORTH);
    }
    return false;
}

bool MouseInterface::wallLeft() {
    switch (m_mouse->getDiscretizedRotation()) {
        case NORTH:
            return m_mouse->discretizedIsWall(WEST);
        case EAST:
            return m_mouse->discretizedIsWall(NORTH);
        case SOUTH:
            return m_mouse->discretizedIsWall(EAST);
        case WEST:
            return m_mouse->discretizedIsWall(SOUTH);
    }
    return false;
}

void MouseInterface::moveForward() {

    // TODO: Check for collision

    std::pair<int, int> currentPosition = m_mouse->getDiscretizedTranslation();
    Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    Meters x = tileLength * (currentPosition.first + 0.5);
    Meters y = tileLength * (currentPosition.second + 0.5);

    switch (m_mouse->getDiscretizedRotation()) {
        case NORTH:
            for (float i = 0; i < 1.0; i += .01) {
                m_mouse->teleport(Cartesian(x, y + tileLength*i), Degrees(0));
                sim::sleep(Milliseconds(1));
            }
            break;
        case EAST:
            for (float i = 0; i < 1.0; i += .01) {
                m_mouse->teleport(Cartesian(x + tileLength*i, y), Degrees(270));
                sim::sleep(Milliseconds(1));
            }
            break;
        case SOUTH:
            for (float i = 0; i < 1.0; i += .01) {
                m_mouse->teleport(Cartesian(x, y - tileLength*i), Degrees(180));
                sim::sleep(Milliseconds(1));
            }
            break;
        case WEST:
            for (float i = 0; i < 1.0; i += .01) {
                m_mouse->teleport(Cartesian(x - tileLength*i, y), Degrees(90));
                sim::sleep(Milliseconds(1));
            }
            break;
    }
}

void MouseInterface::turnRight() {

    std::pair<int, int> currentPosition = m_mouse->getDiscretizedTranslation();
    Meters x = Meters(P()->wallLength() + P()->wallWidth()) * (currentPosition.first + 0.5);
    Meters y = Meters(P()->wallLength() + P()->wallWidth()) * (currentPosition.second + 0.5);

    switch (m_mouse->getDiscretizedRotation()) {
        case NORTH:
            for (float i = 0; i < 1.0; i += .01) {
                m_mouse->teleport(Cartesian(x, y), Degrees(360) - Degrees(90)*i);
                sim::sleep(Milliseconds(1));
            }
            break;
        case EAST:
            for (float i = 0; i < 1.0; i += .01) {
                m_mouse->teleport(Cartesian(x, y), Degrees(270) - Degrees(90)*i);
                sim::sleep(Milliseconds(1));
            }
            break;
        case SOUTH:
            for (float i = 0; i < 1.0; i += .01) {
                m_mouse->teleport(Cartesian(x, y), Degrees(180) - Degrees(90)*i);
                sim::sleep(Milliseconds(1));
            }
            break;
        case WEST:
            for (float i = 0; i < 1.0; i += .01) {
                m_mouse->teleport(Cartesian(x, y), Degrees(90) - Degrees(90)*i);
                sim::sleep(Milliseconds(1));
            }
            break;
    }
}

void MouseInterface::turnLeft() {

    std::pair<int, int> currentPosition = m_mouse->getDiscretizedTranslation();
    Meters x = Meters(P()->wallLength() + P()->wallWidth()) * (currentPosition.first + 0.5);
    Meters y = Meters(P()->wallLength() + P()->wallWidth()) * (currentPosition.second + 0.5);

    switch (m_mouse->getDiscretizedRotation()) {
        case NORTH:
            for (float i = 0; i < 1.0; i += .01) {
                m_mouse->teleport(Cartesian(x, y), Degrees(0) + Degrees(90)*i);
                sim::sleep(Milliseconds(1));
            }
            break;
        case EAST:
            for (float i = 0; i < 1.0; i += .01) {
                m_mouse->teleport(Cartesian(x, y), Degrees(270) + Degrees(90)*i);
                sim::sleep(Milliseconds(1));
            }
            break;
        case SOUTH:
            for (float i = 0; i < 1.0; i += .01) {
                m_mouse->teleport(Cartesian(x, y), Degrees(180) + Degrees(90)*i);
                sim::sleep(Milliseconds(1));
            }
            break;
        case WEST:
            for (float i = 0; i < 1.0; i += .01) {
                m_mouse->teleport(Cartesian(x, y), Degrees(90) + Degrees(90)*i);
                sim::sleep(Milliseconds(1));
            }
            break;
    }
}

void MouseInterface::turnAround() {
    turnRight();
    turnRight();
}

bool MouseInterface::undoRequested() {
    return S()->undoRequested();
}

bool MouseInterface::resetRequested() {
    return S()->resetRequested();
}

void MouseInterface::undoHonored() {
    S()->setUndoRequested(false);
}

void MouseInterface::resetHonored() {
    S()->setResetRequested(false);
}

void MouseInterface::resetPosition() {
    //m_mouse->resetPosition();
}

void MouseInterface::resetColors(int curX, int curY) {
    //m_mouse->resetColors(curX, curY);
}

} // namespace sim

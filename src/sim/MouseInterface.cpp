#include "MouseInterface.h"

#include <cmath>
#include <GL/freeglut.h>

#include <Meters.h>
#include <MetersPerSecond.h>
#include <Milliseconds.h>
#include <Seconds.h>

#include "Assert.h"
#include "Param.h"
#include "State.h"
#include "SimUtilities.h"

#include <iostream> // TODO

// TODO: Diagonals, more discrete interface methods (look ahead), change color of tile, reset, etc, reduce CPU, etc.

namespace sim {

// TODO: For the mouse interface, we assume that the mouse can rotate freely in the square

MouseInterface::MouseInterface(const Maze* maze, Mouse* mouse) : m_maze(maze), m_mouse(mouse) {
}

MouseInterface::~MouseInterface() {
}

void MouseInterface::declareInterfaceType(InterfaceType interfaceType) {

    if (interfaceType == UNDECLARED) {
        print("Error - you may not declare the mouse interface type to be UNDECLARED.");
        quit();
    }

    if (S()->interfaceType() != UNDECLARED) {
        print("Error - you may only declare the mouse interface type once.");
        quit();
    }

    // Actually set the interface type
    S()->setInterfaceType(interfaceType);

    // Once the interface type is determined, we can initialize the collision polygon
    m_mouse->initializeCollisionPolygon();

    // Wait for everything to stabilize
    sim::sleep(Seconds(P()->glutInitTime()));
}

void MouseInterface::delay(int milliseconds) {

    ENSURE_DECLARED

    sim::sleep(Milliseconds(milliseconds));
}

void MouseInterface::setWheelSpeeds(float leftWheelRadiansPerSecond, float rightWheelRadiansPerSecond) {

    ENSURE_CONTINUOUS

    m_mouse->setWheelSpeeds(RadiansPerSecond(leftWheelRadiansPerSecond), RadiansPerSecond(rightWheelRadiansPerSecond));
}

float MouseInterface::read(std::string name) {

    ENSURE_CONTINUOUS

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

bool MouseInterface::wallFront() {

    ENSURE_DISCRETE

    return isWall(getDiscretizedTranslation(), getDiscretizedRotation());
}

bool MouseInterface::wallRight() {

    ENSURE_DISCRETE

    std::pair<int, int> position = getDiscretizedTranslation();

    switch (getDiscretizedRotation()) {
        case NORTH:
            return isWall(position, EAST);
        case EAST:
            return isWall(position, SOUTH);
        case SOUTH:
            return isWall(position, WEST);
        case WEST:
            return isWall(position, NORTH);
    }
}

bool MouseInterface::wallLeft() {

    ENSURE_DISCRETE

    std::pair<int, int> position = getDiscretizedTranslation();

    switch (getDiscretizedRotation()) {
        case NORTH:
            return isWall(position, WEST);
        case EAST:
            return isWall(position, NORTH);
        case SOUTH:
            return isWall(position, EAST);
        case WEST:
            return isWall(position, SOUTH);
    }
}

void MouseInterface::moveForward() {

    ENSURE_DISCRETE

    if (wallFront()) {
        if (!S()->crashed()) {
            S()->setCrashed();
            // TODO: Animation
        }
        return;
    }

    Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    Meters currentX = tileLength * (getDiscretizedTranslation().first) + m_mouse->getInitialTranslation().getX();
    Meters currentY = tileLength * (getDiscretizedTranslation().second) + m_mouse->getInitialTranslation().getY();
    Cartesian destinationTranslation = Cartesian(currentX, currentY);
    Degrees destinationRotation = m_mouse->getCurrentRotation();

    switch (getDiscretizedRotation()) {
        case NORTH: {
            destinationTranslation += Cartesian(Meters(0), tileLength);
            destinationRotation = Degrees(0);
            while (m_mouse->getCurrentTranslation().getY() < destinationTranslation.getY()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()), RadiansPerSecond(S()->simSpeed()));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepTime()));
            }
            break;
        }
        case EAST: {
            destinationTranslation += Cartesian(tileLength, Meters(0));
            destinationRotation = Degrees(270);
            while (m_mouse->getCurrentTranslation().getX() < destinationTranslation.getX()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()), RadiansPerSecond(S()->simSpeed()));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepTime()));
            }
            break;
        }
        case SOUTH: {
            destinationTranslation += Cartesian(Meters(0), tileLength * -1);
            destinationRotation = Degrees(180);
            while (destinationTranslation.getY() < m_mouse->getCurrentTranslation().getY()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()), RadiansPerSecond(S()->simSpeed()));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepTime()));
            }
            break;
        }
        case WEST: {
            destinationTranslation += Cartesian(tileLength * -1, Meters(0));
            destinationRotation = Degrees(90);
            while (destinationTranslation.getX() < m_mouse->getCurrentTranslation().getX()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()), RadiansPerSecond(S()->simSpeed()));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepTime()));
            }
            break;
        }
    }

    m_mouse->setWheelSpeeds(RadiansPerSecond(0), RadiansPerSecond(0));
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::turnRight() {

    ENSURE_DISCRETE

    Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    Cartesian destinationTranslation = m_mouse->getCurrentTranslation();
    Degrees destinationRotation = m_mouse->getCurrentRotation() - Degrees(90);

    switch (getDiscretizedRotation()) {
        case NORTH: {
            while (destinationRotation < m_mouse->getCurrentRotation() || m_mouse->getCurrentRotation() < Degrees(180)) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()/2.0), RadiansPerSecond(-S()->simSpeed()/2.0));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepTime()));
            }
            break;
        }
        case EAST:
        case SOUTH: {
            while (destinationRotation < m_mouse->getCurrentRotation()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()/2.0), RadiansPerSecond(-S()->simSpeed()/2.0));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepTime()));
            }
            break;
        }
        case WEST: {
            while (m_mouse->getCurrentRotation() < Degrees(180)) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()/2.0), RadiansPerSecond(-S()->simSpeed()/2.0));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepTime()));
            }
            break;
        }
    }

    m_mouse->setWheelSpeeds(RadiansPerSecond(0), RadiansPerSecond(0));
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::turnLeft() {

    ENSURE_DISCRETE

    Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    Cartesian destinationTranslation = m_mouse->getCurrentTranslation();
    Degrees destinationRotation = m_mouse->getCurrentRotation() + Degrees(90);

    switch (getDiscretizedRotation()) {
        case NORTH: {
            while (m_mouse->getCurrentRotation() < destinationRotation ||  Degrees(180) < m_mouse->getCurrentRotation()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(S()->simSpeed()/2.0), RadiansPerSecond(S()->simSpeed()/2.0));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepTime()));
            }
            break;
        }
        case EAST: {
            while (Degrees(180) < m_mouse->getCurrentRotation()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(S()->simSpeed()/2.0), RadiansPerSecond(S()->simSpeed()/2.0));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepTime()));
            }
            break;
        }
        case SOUTH:
        case WEST: {
            while (m_mouse->getCurrentRotation() < destinationRotation) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(S()->simSpeed()/2.0), RadiansPerSecond(S()->simSpeed()/2.0));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepTime()));
            }
            break;
        }
    }

    m_mouse->setWheelSpeeds(RadiansPerSecond(0), RadiansPerSecond(0));
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::turnAround() {

    ENSURE_DISCRETE

    turnRight();
    turnRight();
}

bool MouseInterface::resetRequested() {
    return S()->resetRequested();
}

bool MouseInterface::undoRequested() {
    return S()->undoRequested();
}

void MouseInterface::resetHonored() {
    S()->setResetRequested(false);
}

void MouseInterface::undoHonored() {
    S()->setUndoRequested(false);
}

void MouseInterface::resetPosition() {
    //m_mouse->resetPosition();
}

void MouseInterface::resetColors(int curX, int curY) {
    //m_mouse->resetColors(curX, curY);
}

void MouseInterface::ensureDeclaredInterface(const std::string& callingFunction) const {
    if (S()->interfaceType() == UNDECLARED) {
        print(std::string("Error - you must declare the interface type before you can use MouseInterface::")
            + callingFunction + std::string("()."));
        quit();
    }
}

void MouseInterface::ensureDiscreteInterface(const std::string& callingFunction) const {
    if (S()->interfaceType() != DISCRETE) {
        print(std::string("Error - you must declare the interface type to be sim::DISCRETE to use MouseInterface::")
            + callingFunction + std::string("()."));
        quit();
    }
}

void MouseInterface::ensureContinuousInterface(const std::string& callingFunction) const {
    if (S()->interfaceType() != CONTINUOUS) {
        print(std::string("Error - you must declare the interface type to be sim::CONTINUOUS to use MouseInterface::")
            + callingFunction + std::string("()."));
        quit();
    }
}

std::pair<int, int> MouseInterface::getDiscretizedTranslation() const {
    int x = static_cast<int>(floor((m_mouse->getCurrentTranslation().getX() / Meters(P()->wallLength() + P()->wallWidth()))));
    int y = static_cast<int>(floor((m_mouse->getCurrentTranslation().getY() / Meters(P()->wallLength() + P()->wallWidth()))));
    return std::make_pair(x, y);
}

Direction MouseInterface::getDiscretizedRotation() const {
    int dir = static_cast<int>(floor((m_mouse->getCurrentRotation() + Degrees(45)) / Degrees(90)));
    switch (dir) {
        case 0:
            return NORTH;
        case 1:
            return WEST;
        case 2:
            return SOUTH;
        case 3:
            return EAST;
    }
}

void MouseInterface::checkPaused() {
    if (S()->paused()) {
        m_mouse->setWheelSpeeds(RadiansPerSecond(0), RadiansPerSecond(0));
        while (S()->paused()) {
            sim::sleep(Milliseconds(P()->discreteInterfaceSleepTime()));
        }
    }
}

bool MouseInterface::isWall(std::pair<int, int> position, Direction direction) const {
    ASSERT(0 <= position.first && position.first < m_maze->getWidth()
        && 0 <= position.second && position.second < m_maze->getHeight());
    return m_maze->getTile(position.first, position.second)->isWall(direction);
}

} // namespace sim

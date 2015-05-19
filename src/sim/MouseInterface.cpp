#include "MouseInterface.h"

#include <cmath>
#include <glut.h>

#include <Meters.h>
#include <MetersPerSecond.h>
#include <Milliseconds.h>
#include <Seconds.h>

#include "Assert.h"
#include "Colors.h"
#include "Param.h"
#include "State.h"
#include "SimUtilities.h"

namespace sim {

MouseInterface::MouseInterface(const Maze* maze, Mouse* mouse, MazeGraphic* mazeGraphic) :
        m_maze(maze), m_mouse(mouse), m_mazeGraphic(mazeGraphic) {
}

MouseInterface::~MouseInterface() {
}

void MouseInterface::initializeMouse(const std::string& mouseFile) {

    if (m_mouse->getInitialized()) {
        print("Error: You may only initialize the mouse once.");
        quit();
    }

    if (!m_mouse->initialize(mouseFile)) {
        print("Error: Unable to successfully initialize the mouse from \"" + mouseFile + "\".");
        quit();
    }
}

void MouseInterface::declareInterfaceType(InterfaceType interfaceType) {

    ENSURE_INITIALIZED_MOUSE

    if (interfaceType == UNDECLARED) {
        print("Error: You may not declare the mouse interface type to be UNDECLARED.");
        quit();
    }

    if (S()->interfaceType() != UNDECLARED) {
        print("Error: You may only declare the mouse interface type once.");
        quit();
    }

    // Actually set the interface type
    S()->setInterfaceType(interfaceType);

    // Once the interface type is determined, we can initialize the collision polygon
    m_mouse->initializeCollisionPolygon();

    // Wait for everything to stabilize
    sim::sleep(Seconds(P()->glutInitDuration()));
}

void MouseInterface::delay(int milliseconds) {

    ENSURE_INITIALIZED_MOUSE
    ENSURE_DECLARED_INTERFACE

    sim::sleep(Milliseconds(milliseconds));
}

void MouseInterface::colorTile(int x, int y, char color) {

    ENSURE_INITIALIZED_MOUSE
    ENSURE_DECLARED_INTERFACE

    if (x < 0 || m_mazeGraphic->getWidth() < x || y < 0 || m_mazeGraphic->getHeight() <= y) {
        print(std::string("Error: There is no tile at position (") + std::to_string(x) + std::string(", ")
            + std::to_string(y) + std::string("), and thus you cannot set its color."));
        return;
    }

    if (COLOR_CHARS.find(color) == COLOR_CHARS.end()) {
        print(std::string("Error: The character '") + std::to_string(color) + std::string("' is not mapped to a color."));
        return;
    }

    m_mazeGraphic->setColor(x, y, COLOR_CHARS.at(color));
    m_coloredTiles.insert(std::make_pair(x, y));
}

void MouseInterface::resetColors() {

    ENSURE_INITIALIZED_MOUSE
    ENSURE_DECLARED_INTERFACE

    for (std::pair<int, int> position : m_coloredTiles) {
        m_mazeGraphic->setColor(position.first, position.second, COLOR_STRINGS.at(P()->tileBaseColor()));
    }
    m_coloredTiles.clear();
}

void MouseInterface::declareWall(int x, int y, char direction, bool wallExists) {

    ENSURE_INITIALIZED_MOUSE
    ENSURE_DECLARED_INTERFACE

    if (x < 0 || m_mazeGraphic->getWidth() < x || y < 0 || m_mazeGraphic->getHeight() <= y) {
        print(std::string("Error: There is no tile at position (") + std::to_string(x) + std::string(", ")
            + std::to_string(y) + std::string("), and thus you cannot declare any of its walls."));
        return;
    }

    switch (direction) {
        case 'n':
            m_mazeGraphic->setAlgoWall(x, y, NORTH, wallExists); 
            break;
        case 'e':
            m_mazeGraphic->setAlgoWall(x, y, EAST, wallExists); 
            break;
        case 's':
            m_mazeGraphic->setAlgoWall(x, y, SOUTH, wallExists); 
            break;
        case 'w':
            m_mazeGraphic->setAlgoWall(x, y, WEST, wallExists); 
            break;
        default:
            print(std::string("The character '") + std::to_string(direction) + std::string("' is not mapped to a valid direction"));
            return;
    }

    if (P()->declareBothWallHalves()) {
        switch (direction) {
            case 'n':
                if (y < m_maze->getHeight() - 1) {
                    m_mazeGraphic->setAlgoWall(x, y + 1, SOUTH, wallExists); 
                }
                break;
            case 'e':
                if (x < m_maze->getWidth() - 1) {
                    m_mazeGraphic->setAlgoWall(x + 1, y, WEST, wallExists); 
                }
                break;
            case 's':
                if (y > 0) {
                    m_mazeGraphic->setAlgoWall(x, y - 1, NORTH, wallExists); 
                }
                break;
            case 'w':
                if (x > 0) {
                    m_mazeGraphic->setAlgoWall(x - 1, y, EAST, wallExists); 
                }
                break;
        }
    }
}

void MouseInterface::resetPosition() {

    ENSURE_INITIALIZED_MOUSE
    ENSURE_DECLARED_INTERFACE

    m_mouse->teleport(m_mouse->getInitialTranslation(), Radians(0.0));
}

bool MouseInterface::inputButtonPressed(int inputButton) {

    ENSURE_INITIALIZED_MOUSE
    ENSURE_DECLARED_INTERFACE

    if (inputButton < 0 || 9 < inputButton) {
        print(std::string("Error: There is no input button with the number ") + std::to_string(inputButton)
            + std::string(", and thus you cannot check to see if it has been pressed."));
        return false;
    }

    return S()->inputButtonPressed(inputButton);
}

void MouseInterface::acknowledgeInputButtonPressed(int inputButton) {

    ENSURE_INITIALIZED_MOUSE
    ENSURE_DECLARED_INTERFACE

    if (inputButton < 0 || 9 < inputButton) {
        print(std::string("Error: There is no input button with the number ") + std::to_string(inputButton)
            + std::string(", and thus you cannot acknowledge that it has been pressed."));
        return;
    }

    S()->setInputButtonPressed(inputButton, false);
}

void MouseInterface::setWheelSpeeds(float leftWheelRadiansPerSecond, float rightWheelRadiansPerSecond) {

    ENSURE_INITIALIZED_MOUSE
    ENSURE_CONTINUOUS_INTERFACE

    m_mouse->setWheelSpeeds(RadiansPerSecond(leftWheelRadiansPerSecond), RadiansPerSecond(rightWheelRadiansPerSecond));
}

float MouseInterface::read(std::string name) {

    ENSURE_INITIALIZED_MOUSE
    ENSURE_CONTINUOUS_INTERFACE

    if (!m_mouse->hasSensor(name)) {
        print(std::string("Error: There is no sensor called \"") + std::string(name)
            + std::string("\" and thus you cannot read its value."));
        return 0.0;
    }

    // Start the timer
    double start(sim::getHighResTime());

    // Retrieve the value
    float value = m_mouse->read(name);

    // Stop the timer
    double end(sim::getHighResTime());
    double duration = end - start;

    // Display to the user, if requested
    if (sim::P()->printLateSensorReads() && duration > m_mouse->getReadDuration(name).getSeconds()) {
        sim::print(std::string("A sensor read was late by ")
            + std::to_string(duration - m_mouse->getReadDuration(name).getSeconds())
            + std::string(" seconds, which is ")
            + std::to_string((duration - m_mouse->getReadDuration(name).getSeconds())/(m_mouse->getReadDuration(name).getSeconds()) * 100)
            + std::string(" percent late."));
    }

    // Sleep for the read time
    sim::sleep(sim::Seconds(std::max(0.0, 1.0/sim::P()->frameRate() - duration)));

    // Return the value
    return value;
}

bool MouseInterface::wallFront() {

    ENSURE_INITIALIZED_MOUSE
    ENSURE_DISCRETE_INTERFACE

    return isWall(getDiscretizedTranslation(), getDiscretizedRotation());
}

bool MouseInterface::wallRight() {

    ENSURE_INITIALIZED_MOUSE
    ENSURE_DISCRETE_INTERFACE

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

    ENSURE_INITIALIZED_MOUSE
    ENSURE_DISCRETE_INTERFACE

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

    ENSURE_INITIALIZED_MOUSE
    ENSURE_DISCRETE_INTERFACE

    if (wallFront()) {
        if (!S()->crashed()) {
            S()->setCrashed();
        }
        return;
    }

    Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    Meters currentX = tileLength * (getDiscretizedTranslation().first) + m_mouse->getInitialTranslation().getX();
    Meters currentY = tileLength * (getDiscretizedTranslation().second) + m_mouse->getInitialTranslation().getY();

    // We modify these values in the switch statement
    Cartesian destinationTranslation = Cartesian(currentX, currentY);
    Degrees destinationRotation(0.0);

    switch (getDiscretizedRotation()) {
        case NORTH: {
            destinationTranslation += Cartesian(Meters(0), tileLength);
            destinationRotation = Degrees(0);
            while (m_mouse->getCurrentTranslation().getY() < destinationTranslation.getY()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()), RadiansPerSecond(S()->simSpeed()));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepDuration()));
            }
            break;
        }
        case EAST: {
            destinationTranslation += Cartesian(tileLength, Meters(0));
            destinationRotation = Degrees(270);
            while (m_mouse->getCurrentTranslation().getX() < destinationTranslation.getX()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()), RadiansPerSecond(S()->simSpeed()));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepDuration()));
            }
            break;
        }
        case SOUTH: {
            destinationTranslation += Cartesian(Meters(0), tileLength * -1);
            destinationRotation = Degrees(180);
            while (destinationTranslation.getY() < m_mouse->getCurrentTranslation().getY()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()), RadiansPerSecond(S()->simSpeed()));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepDuration()));
            }
            break;
        }
        case WEST: {
            destinationTranslation += Cartesian(tileLength * -1, Meters(0));
            destinationRotation = Degrees(90);
            while (destinationTranslation.getX() < m_mouse->getCurrentTranslation().getX()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()), RadiansPerSecond(S()->simSpeed()));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepDuration()));
            }
            break;
        }
    }

    m_mouse->setWheelSpeeds(RadiansPerSecond(0), RadiansPerSecond(0));
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::turnRight() {

    ENSURE_INITIALIZED_MOUSE
    ENSURE_DISCRETE_INTERFACE

    Cartesian destinationTranslation = m_mouse->getCurrentTranslation();
    Degrees destinationRotation = m_mouse->getCurrentRotation() - Degrees(90);

    switch (getDiscretizedRotation()) {
        case NORTH: {
            while (destinationRotation < m_mouse->getCurrentRotation() || m_mouse->getCurrentRotation() < Degrees(180)) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()/2.0), RadiansPerSecond(-S()->simSpeed()/2.0));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepDuration()));
            }
            break;
        }
        case EAST:
        case SOUTH: {
            while (destinationRotation < m_mouse->getCurrentRotation()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()/2.0), RadiansPerSecond(-S()->simSpeed()/2.0));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepDuration()));
            }
            break;
        }
        case WEST: {
            while (m_mouse->getCurrentRotation() < Degrees(180)) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()/2.0), RadiansPerSecond(-S()->simSpeed()/2.0));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepDuration()));
            }
            break;
        }
    }

    m_mouse->setWheelSpeeds(RadiansPerSecond(0), RadiansPerSecond(0));
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::turnLeft() {

    ENSURE_INITIALIZED_MOUSE
    ENSURE_DISCRETE_INTERFACE

    Cartesian destinationTranslation = m_mouse->getCurrentTranslation();
    Degrees destinationRotation = m_mouse->getCurrentRotation() + Degrees(90);

    switch (getDiscretizedRotation()) {
        case NORTH: {
            while (m_mouse->getCurrentRotation() < destinationRotation ||  Degrees(180) < m_mouse->getCurrentRotation()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(S()->simSpeed()/2.0), RadiansPerSecond(S()->simSpeed()/2.0));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepDuration()));
            }
            break;
        }
        case EAST: {
            while (Degrees(180) < m_mouse->getCurrentRotation()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(S()->simSpeed()/2.0), RadiansPerSecond(S()->simSpeed()/2.0));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepDuration()));
            }
            break;
        }
        case SOUTH:
        case WEST: {
            while (m_mouse->getCurrentRotation() < destinationRotation) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(S()->simSpeed()/2.0), RadiansPerSecond(S()->simSpeed()/2.0));
                sim::sleep(Milliseconds(P()->discreteInterfaceSleepDuration()));
            }
            break;
        }
    }

    m_mouse->setWheelSpeeds(RadiansPerSecond(0), RadiansPerSecond(0));
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::turnAround() {

    ENSURE_INITIALIZED_MOUSE
    ENSURE_DISCRETE_INTERFACE

    turnRight();
    turnRight();
}

void MouseInterface::ensureInitializedMouse(const std::string& callingFunction) const {
    if (!m_mouse->getInitialized()) {
        print(std::string("Error: You must successfully initialize the mouse before you can use MouseInterface::")
            + callingFunction + std::string("()."));
        quit();
    }
}

void MouseInterface::ensureDeclaredInterface(const std::string& callingFunction) const {
    if (S()->interfaceType() == UNDECLARED) {
        print(std::string("Error: You must declare the interface type before you can use MouseInterface::")
            + callingFunction + std::string("()."));
        quit();
    }
}

void MouseInterface::ensureDiscreteInterface(const std::string& callingFunction) const {
    if (S()->interfaceType() != DISCRETE) {
        print(std::string("Error: You must declare the interface type to be sim::DISCRETE to use MouseInterface::")
            + callingFunction + std::string("()."));
        quit();
    }
}

void MouseInterface::ensureContinuousInterface(const std::string& callingFunction) const {
    if (S()->interfaceType() != CONTINUOUS) {
        print(std::string("Error: You must declare the interface type to be sim::CONTINUOUS to use MouseInterface::")
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
            sim::sleep(Milliseconds(P()->discreteInterfaceSleepDuration()));
        }
    }
}

bool MouseInterface::isWall(std::pair<int, int> position, Direction direction) {

    ASSERT(0 <= position.first && position.first < m_maze->getWidth()
        && 0 <= position.second && position.second < m_maze->getHeight());

    bool wallExists = m_maze->getTile(position.first, position.second)->isWall(direction);

    if (P()->discreteInterfaceDeclareWallOnRead()) {
        switch (direction) {
            case NORTH:
                declareWall(position.first, position.second, 'n', wallExists);
                break;
            case EAST:
                declareWall(position.first, position.second, 'e', wallExists);
                break;
            case SOUTH:
                declareWall(position.first, position.second, 's', wallExists);
                break;
            case WEST:
                declareWall(position.first, position.second, 'w', wallExists);
                break;
        }
    }

    return wallExists;
}

} // namespace sim

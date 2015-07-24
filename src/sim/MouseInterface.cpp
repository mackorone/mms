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

void MouseInterface::delay(int milliseconds) {
    sim::SimUtilities::sleep(Milliseconds(milliseconds));
}

void MouseInterface::setTileColor(int x, int y, char color) {

    if (x < 0 || m_mazeGraphic->getWidth() <= x || y < 0 || m_mazeGraphic->getHeight() <= y) {
        SimUtilities::print(std::string("Error: There is no tile at position (") + std::to_string(x) + std::string(", ")
            + std::to_string(y) + std::string("), and thus you cannot set its color."));
        return;
    }

    if (COLOR_CHARS.find(color) == COLOR_CHARS.end()) {
        SimUtilities::print(std::string("Error: The character '") + color + std::string("' is not mapped to a color."));
        return;
    }

    m_mazeGraphic->setTileColor(x, y, COLOR_CHARS.at(color));
    m_tilesWithColor.insert(std::make_pair(x, y));
}

void MouseInterface::clearTileColor(int x, int y) {

    if (x < 0 || m_mazeGraphic->getWidth() <= x || y < 0 || m_mazeGraphic->getHeight() <= y) {
        SimUtilities::print(std::string("Error: There is no tile at position (") + std::to_string(x) + std::string(", ")
            + std::to_string(y) + std::string("), and thus you cannot clear its color."));
        return;
    }

    m_mazeGraphic->setTileColor(x, y, COLOR_STRINGS.at(P()->tileBaseColor()));
    m_tilesWithColor.erase(std::make_pair(x, y));
}

void MouseInterface::clearAllTileColor() {

    for (std::pair<int, int> position : m_tilesWithColor) {
        m_mazeGraphic->setTileColor(position.first, position.second, COLOR_STRINGS.at(P()->tileBaseColor()));
    }
    m_tilesWithColor.clear();
}

void MouseInterface::setTileDistance(int x, int y, int distance) {

    if (x < 0 || m_mazeGraphic->getWidth() <= x || y < 0 || m_mazeGraphic->getHeight() <= y) {
        SimUtilities::print(std::string("Error: There is no tile at position (") + std::to_string(x) + std::string(", ")
            + std::to_string(y) + std::string("), and thus you cannot set its distance."));
        return;
    }

    if (distance < 0 || 255 < distance) {
        SimUtilities::print(std::string("Error: You cannot set the distance of the tile at position ("
            + std::to_string(x) + std::string(", ") + std::to_string(y) + std::string(") to " + std::to_string(distance)
            + " since that falls outside of [0,255], the range of valid tile distances.")));
        return;
    }

    m_mazeGraphic->setTileDistance(x, y, distance);
}

void MouseInterface::clearTileDistance(int x, int y) {

    if (x < 0 || m_mazeGraphic->getWidth() <= x || y < 0 || m_mazeGraphic->getHeight() <= y) {
        SimUtilities::print(std::string("Error: There is no tile at position (") + std::to_string(x) + std::string(", ")
            + std::to_string(y) + std::string("), and thus you cannot clear its distance."));
        return;
    }

    m_mazeGraphic->setTileDistance(x, y, -1);
}

void MouseInterface::clearAllTileDistance() {

    for (int x = 0; x < m_mazeGraphic->getWidth(); x += 1) {
        for (int y = 0; y < m_mazeGraphic->getHeight(); y += 1) {
            m_mazeGraphic->setTileDistance(x, y, -1); 
        }
    }
}

void MouseInterface::setTileFogginess(int x, int y, bool foggy) {

    if (x < 0 || m_mazeGraphic->getWidth() <= x || y < 0 || m_mazeGraphic->getHeight() <= y) {
        SimUtilities::print(std::string("Error: There is no tile at position (") + std::to_string(x) + std::string(", ")
            + std::to_string(y) + std::string("), and thus you cannot set its fogginess."));
        return;
    }

    m_mazeGraphic->setTileFogginess(x, y, foggy);
}

void MouseInterface::declareWall(int x, int y, char direction, bool wallExists) {

    if (x < 0 || m_mazeGraphic->getWidth() <= x || y < 0 || m_mazeGraphic->getHeight() <= y) {
        SimUtilities::print(std::string("Error: There is no tile at position (") + std::to_string(x) + std::string(", ")
            + std::to_string(y) + std::string("), and thus you cannot declare any of its walls."));
        return;
    }

    switch (direction) {
        case 'n':
            m_mazeGraphic->declareWall(x, y, NORTH, wallExists); 
            break;
        case 'e':
            m_mazeGraphic->declareWall(x, y, EAST, wallExists); 
            break;
        case 's':
            m_mazeGraphic->declareWall(x, y, SOUTH, wallExists); 
            break;
        case 'w':
            m_mazeGraphic->declareWall(x, y, WEST, wallExists); 
            break;
        default:
            SimUtilities::print(std::string("The character '") + direction + std::string("' is not mapped to a valid direction"));
            return;
    }

    if (P()->declareBothWallHalves()) {
        switch (direction) {
            case 'n':
                if (y < m_maze->getHeight() - 1) {
                    m_mazeGraphic->declareWall(x, y + 1, SOUTH, wallExists); 
                }
                break;
            case 'e':
                if (x < m_maze->getWidth() - 1) {
                    m_mazeGraphic->declareWall(x + 1, y, WEST, wallExists); 
                }
                break;
            case 's':
                if (y > 0) {
                    m_mazeGraphic->declareWall(x, y - 1, NORTH, wallExists); 
                }
                break;
            case 'w':
                if (x > 0) {
                    m_mazeGraphic->declareWall(x - 1, y, EAST, wallExists); 
                }
                break;
        }
    }
}

void MouseInterface::undeclareWall(int x, int y, char direction) {

    if (x < 0 || m_mazeGraphic->getWidth() <= x || y < 0 || m_mazeGraphic->getHeight() <= y) {
        SimUtilities::print(std::string("Error: There is no tile at position (") + std::to_string(x) + std::string(", ")
            + std::to_string(y) + std::string("), and thus you cannot undeclare any of its walls."));
        return;
    }

    switch (direction) {
        case 'n':
            m_mazeGraphic->undeclareWall(x, y, NORTH); 
            break;
        case 'e':
            m_mazeGraphic->undeclareWall(x, y, EAST); 
            break;
        case 's':
            m_mazeGraphic->undeclareWall(x, y, SOUTH); 
            break;
        case 'w':
            m_mazeGraphic->undeclareWall(x, y, WEST); 
            break;
        default:
            SimUtilities::print(std::string("The character '") + direction + std::string("' is not mapped to a valid direction"));
            return;
    }

    if (P()->declareBothWallHalves()) {
        switch (direction) {
            case 'n':
                if (y < m_maze->getHeight() - 1) {
                    m_mazeGraphic->undeclareWall(x, y + 1, SOUTH); 
                }
                break;
            case 'e':
                if (x < m_maze->getWidth() - 1) {
                    m_mazeGraphic->undeclareWall(x + 1, y, WEST); 
                }
                break;
            case 's':
                if (y > 0) {
                    m_mazeGraphic->undeclareWall(x, y - 1, NORTH); 
                }
                break;
            case 'w':
                if (x > 0) {
                    m_mazeGraphic->undeclareWall(x - 1, y, EAST); 
                }
                break;
        }
    }
}

void MouseInterface::resetPosition() {
    m_mouse->teleport(m_mouse->getInitialTranslation(), Radians(0.0));
}

bool MouseInterface::inputButtonPressed(int inputButton) {

    if (inputButton < 0 || 9 < inputButton) {
        SimUtilities::print(std::string("Error: There is no input button with the number ") + std::to_string(inputButton)
            + std::string(", and thus you cannot check to see if it has been pressed."));
        return false;
    }

    return S()->inputButtonWasPressed(inputButton);
}

void MouseInterface::acknowledgeInputButtonPressed(int inputButton) {

    if (inputButton < 0 || 9 < inputButton) {
        SimUtilities::print(std::string("Error: There is no input button with the number ") + std::to_string(inputButton)
            + std::string(", and thus you cannot acknowledge that it has been pressed."));
        return;
    }

    S()->setInputButtonWasPressed(inputButton, false);
}

void MouseInterface::setWheelSpeeds(double leftWheelRadiansPerSecond, double rightWheelRadiansPerSecond) {

    ENSURE_CONTINUOUS_INTERFACE

    m_mouse->setWheelSpeeds(RadiansPerSecond(leftWheelRadiansPerSecond), RadiansPerSecond(rightWheelRadiansPerSecond));
}

double MouseInterface::read(std::string name) {

    ENSURE_CONTINUOUS_INTERFACE

    if (!m_mouse->hasSensor(name)) {
        SimUtilities::print(std::string("Error: There is no sensor called \"") + std::string(name)
            + std::string("\" and thus you cannot read its value."));
        return 0.0;
    }

    // Start the timer
    double start(sim::SimUtilities::getHighResTime());

    // Retrieve the value
    double value = m_mouse->read(name);

    // Stop the timer
    double end(sim::SimUtilities::getHighResTime());
    double duration = end - start;

    // Display to the user, if requested
    if (sim::P()->printLateSensorReads() && duration > m_mouse->getReadDuration(name).getSeconds()) {
        sim::SimUtilities::print(std::string("A sensor read was late by ")
            + std::to_string(duration - m_mouse->getReadDuration(name).getSeconds())
            + std::string(" seconds, which is ")
            + std::to_string((duration - m_mouse->getReadDuration(name).getSeconds())/(m_mouse->getReadDuration(name).getSeconds()) * 100)
            + std::string(" percent late."));
    }

    // Sleep for the read time
    sim::SimUtilities::sleep(sim::Seconds(std::max(0.0, 1.0/sim::P()->frameRate() - duration)));

    // Return the value
    return value;
}

bool MouseInterface::wallFront() {

    ENSURE_DISCRETE_INTERFACE

    return isWall(getDiscretizedTranslation(), getDiscretizedRotation());
}

bool MouseInterface::wallRight() {

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

    ENSURE_DISCRETE_INTERFACE

    if (wallFront()) {
        if (!S()->crashed()) {
            S()->setCrashed();
        }
        return;
    }

    // TODO: Check speed here, skip logic and just teleport if too fast (then sleep for the remaining)
    // TODO: Do the same for other movement methods
    // TODO: Try to increase max speed
    //if (S()->simSpeed() < X) {

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
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
        case EAST: {
            destinationTranslation += Cartesian(tileLength, Meters(0));
            destinationRotation = Degrees(270);
            while (m_mouse->getCurrentTranslation().getX() < destinationTranslation.getX()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()), RadiansPerSecond(S()->simSpeed()));
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
        case SOUTH: {
            destinationTranslation += Cartesian(Meters(0), tileLength * -1);
            destinationRotation = Degrees(180);
            while (destinationTranslation.getY() < m_mouse->getCurrentTranslation().getY()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()), RadiansPerSecond(S()->simSpeed()));
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
        case WEST: {
            destinationTranslation += Cartesian(tileLength * -1, Meters(0));
            destinationRotation = Degrees(90);
            while (destinationTranslation.getX() < m_mouse->getCurrentTranslation().getX()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()), RadiansPerSecond(S()->simSpeed()));
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
    }

    m_mouse->setWheelSpeeds(RadiansPerSecond(0), RadiansPerSecond(0));
    m_mouse->teleport(destinationTranslation, destinationRotation);

    if (P()->discreteInterfaceUnfogTileOnEntry()) {
        m_mazeGraphic->setTileFogginess(getDiscretizedTranslation().first, getDiscretizedTranslation().second, false);
    }
}

void MouseInterface::turnRight() {

    ENSURE_DISCRETE_INTERFACE

    Cartesian destinationTranslation = m_mouse->getCurrentTranslation();
    Degrees destinationRotation = m_mouse->getCurrentRotation() - Degrees(90);

    switch (getDiscretizedRotation()) {
        case NORTH: {
            while (destinationRotation < m_mouse->getCurrentRotation() || m_mouse->getCurrentRotation() < Degrees(180)) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()/2.0), RadiansPerSecond(-S()->simSpeed()/2.0));
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
        case EAST:
        case SOUTH: {
            while (destinationRotation < m_mouse->getCurrentRotation()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()/2.0), RadiansPerSecond(-S()->simSpeed()/2.0));
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
        case WEST: {
            while (m_mouse->getCurrentRotation() < Degrees(180)) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(-S()->simSpeed()/2.0), RadiansPerSecond(-S()->simSpeed()/2.0));
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
    }

    m_mouse->setWheelSpeeds(RadiansPerSecond(0), RadiansPerSecond(0));
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::turnLeft() {

    ENSURE_DISCRETE_INTERFACE

    Cartesian destinationTranslation = m_mouse->getCurrentTranslation();
    Degrees destinationRotation = m_mouse->getCurrentRotation() + Degrees(90);

    switch (getDiscretizedRotation()) {
        case NORTH: {
            while (m_mouse->getCurrentRotation() < destinationRotation ||  Degrees(180) < m_mouse->getCurrentRotation()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(S()->simSpeed()/2.0), RadiansPerSecond(S()->simSpeed()/2.0));
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
        case EAST: {
            while (Degrees(180) < m_mouse->getCurrentRotation()) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(S()->simSpeed()/2.0), RadiansPerSecond(S()->simSpeed()/2.0));
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
        case SOUTH:
        case WEST: {
            while (m_mouse->getCurrentRotation() < destinationRotation) {
                checkPaused();
                m_mouse->setWheelSpeeds(RadiansPerSecond(S()->simSpeed()/2.0), RadiansPerSecond(S()->simSpeed()/2.0));
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
    }

    m_mouse->setWheelSpeeds(RadiansPerSecond(0), RadiansPerSecond(0));
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::turnAround() {

    ENSURE_DISCRETE_INTERFACE

    turnRight();
    turnRight();
}

void MouseInterface::ensureDiscreteInterface(const std::string& callingFunction) const {
    if (S()->interfaceType() != DISCRETE) {
        SimUtilities::print(std::string("Error: You must declare the interface type to be sim::DISCRETE to use MouseInterface::")
            + callingFunction + std::string("()."));
        SimUtilities::quit();
    }
}

void MouseInterface::ensureContinuousInterface(const std::string& callingFunction) const {
    if (S()->interfaceType() != CONTINUOUS) {
        SimUtilities::print(std::string("Error: You must declare the interface type to be sim::CONTINUOUS to use MouseInterface::")
            + callingFunction + std::string("()."));
        SimUtilities::quit();
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

void MouseInterface::prepareForLaunch() {

    // Wait for everything to stabilize
    sim::SimUtilities::sleep(Seconds(P()->glutInitDuration()));

    // Unfog the beginning tile if necessary
    if (S()->interfaceType() == DISCRETE && P()->discreteInterfaceUnfogTileOnEntry()) {
        m_mazeGraphic->setTileFogginess(0, 0, false);
    }
}

void MouseInterface::checkPaused() const {
    if (S()->paused()) {
        m_mouse->setWheelSpeeds(RadiansPerSecond(0), RadiansPerSecond(0));
        while (S()->paused()) {
            sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
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

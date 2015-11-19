#include "MouseInterface.h"

#include "units/Meters.h"
#include "units/MetersPerSecond.h"
#include "units/Milliseconds.h"
#include "units/RevolutionsPerSecond.h"
#include "units/Seconds.h"

#include "Assert.h"
#include "Color.h"
#include "CPMath.h"
#include "Logging.h"
#include "Param.h"
#include "State.h"
#include "SimUtilities.h"

// Needs to be included last for Windows compatibility
#include "CPMinMax.h"

namespace sim {

MouseInterface::MouseInterface(const Maze* maze, Mouse* mouse, MazeGraphic* mazeGraphic) :
        m_maze(maze), m_mouse(mouse), m_mazeGraphic(mazeGraphic) {
}

void MouseInterface::debug(const std::string& str) {
    Logging::getMouseLogger()->debug(str);
}

void MouseInterface::info(const std::string& str) {
    Logging::getMouseLogger()->info(str);
}

void MouseInterface::warn(const std::string& str) {
    Logging::getMouseLogger()->warn(str);
}

void MouseInterface::error(const std::string& str) {
    Logging::getMouseLogger()->error(str);
}

double MouseInterface::getRandom() {
    return SimUtilities::getRandom();
}

void MouseInterface::delay(int milliseconds) {
    sim::SimUtilities::sleep(Milliseconds(milliseconds));
}

void MouseInterface::quit() {
    sim::SimUtilities::quit();
}

void MouseInterface::setTileColor(int x, int y, char color) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v) and thus you cannot set its"
            " color.",
            x, y);
        return;
    }

    if (!SimUtilities::mapContains(CHAR_TO_COLOR, color)) {
        L()->warn(
            "You cannot set the color of tile (%v, %v) to '%v' since '%v' is"
            " not mapped to a color.",
            x, y, color, color);
        return;
    }

    m_mazeGraphic->setTileColor(x, y, CHAR_TO_COLOR.at(color));
    m_tilesWithColor.insert(std::make_pair(x, y));
}

void MouseInterface::clearTileColor(int x, int y) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and thus you cannot clear its"
            " color.",
            x, y);
        return;
    }

    m_mazeGraphic->setTileColor(x, y, STRING_TO_COLOR.at(P()->tileBaseColor()));
    m_tilesWithColor.erase(std::make_pair(x, y));
}

void MouseInterface::clearAllTileColor() {

    for (std::pair<int, int> position : m_tilesWithColor) {
        m_mazeGraphic->setTileColor(position.first, position.second, STRING_TO_COLOR.at(P()->tileBaseColor()));
    }
    m_tilesWithColor.clear();
}

void MouseInterface::declareWall(int x, int y, char direction, bool wallExists) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and thus you cannot declare"
            " any of its walls.", x, y);
        return;
    }

    if (!SimUtilities::mapContains(CHAR_TO_DIRECTION, direction)) {
        L()->warn("The character '%v' is not mapped to a valid direction.", direction);
        return;
    }

    m_mazeGraphic->declareWall(x, y, CHAR_TO_DIRECTION.at(direction), wallExists); 
    if (P()->declareBothWallHalves() && hasOpposingWall(x, y, CHAR_TO_DIRECTION.at(direction))) {
        std::pair<std::pair<int, int>, Direction> opposing = getOpposingWall(x, y, CHAR_TO_DIRECTION.at(direction));
        m_mazeGraphic->declareWall(opposing.first.first, opposing.first.second, opposing.second, wallExists); 
    }
}

void MouseInterface::undeclareWall(int x, int y, char direction) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v) and thus you cannot"
            " undeclare any of its walls.", x, y);
        return;
    }

    if (!SimUtilities::mapContains(CHAR_TO_DIRECTION, direction)) {
        L()->warn("The character '%v' is not mapped to a valid direction.", direction);
        return;
    }

    m_mazeGraphic->undeclareWall(x, y, CHAR_TO_DIRECTION.at(direction));
    if (P()->declareBothWallHalves() && hasOpposingWall(x, y, CHAR_TO_DIRECTION.at(direction))) {
        std::pair<std::pair<int, int>, Direction> opposing = getOpposingWall(x, y, CHAR_TO_DIRECTION.at(direction));
        m_mazeGraphic->undeclareWall(opposing.first.first, opposing.first.second, opposing.second);
    }
}

void MouseInterface::setTileFogginess(int x, int y, bool foggy) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and thus you cannot set"
            " its fogginess.", x, y);
        return;
    }

    if (!P()->algorithmControlsTileFog()) { 
        // TODO: MACK - error statement
        return;
    }

    m_mazeGraphic->setTileFogginess(x, y, foggy);
}

void MouseInterface::declareTileDistance(int x, int y, int distance) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and thus you cannot set"
            " its distance.", x, y);
        return;
    }

    m_mazeGraphic->setTileText(x, y, {std::to_string(distance)});
}

void MouseInterface::undeclareTileDistance(int x, int y) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and thus you cannot clear"
            " its distance.", x, y);
        return;
    }

    m_mazeGraphic->setTileText(x, y, {});
}

void MouseInterface::resetPosition() {
    m_mouse->teleport(m_mouse->getInitialTranslation(), m_mouse->getInitialRotation());
}

bool MouseInterface::inputButtonPressed(int inputButton) {

    if (inputButton < 0 || 9 < inputButton) {
        L()->warn(
            "There is no input button with the number %v, and thus you cannot"
            " check to see if it has been pressed.", inputButton);
        return false;
    }

    return S()->inputButtonWasPressed(inputButton);
}

void MouseInterface::acknowledgeInputButtonPressed(int inputButton) {

    if (inputButton < 0 || 9 < inputButton) {
        L()->warn(
            "There is no input button with the number %v, and thus you cannot"
            " acknowledge that it has been pressed.", inputButton);
        return;
    }

    S()->setInputButtonWasPressed(inputButton, false);
    L()->info("Input button %v was acknowledged as pressed; it can now be pressed again.", inputButton);
}

void MouseInterface::setWheelSpeed(const std::string& name, double radiansPerSecond) {

    ENSURE_CONTINUOUS_INTERFACE

    if (!m_mouse->hasWheel(name)) {
        L()->warn("There is no wheel called \"%v\" and thus you cannot set its speed.", name);
    }

    m_mouse->setWheelSpeeds({{name, radiansPerSecond}});
}

double MouseInterface::read(std::string name) {

    ENSURE_CONTINUOUS_INTERFACE

    if (!m_mouse->hasSensor(name)) {
        L()->warn("There is no sensor called \"%v\" and thus you cannot read its value.", name);
        return 0.0;
    }

    // Start the timer
    double start(sim::SimUtilities::getHighResTime());

    // Retrieve the value
    double value = m_mouse->readSensor(name);

    // Stop the timer
    double end(sim::SimUtilities::getHighResTime());
    double duration = end - start;

    // Display to the user, if requested
    double readDurationSeconds = m_mouse->getSensorReadDuration(name).getSeconds();
    if (sim::P()->printLateSensorReads() && duration > readDurationSeconds) {
        L()->warn(
            "A sensor read was late by %v seconds, which is %v percent late.",
            (duration - readDurationSeconds),
            (duration - readDurationSeconds)/readDurationSeconds * 100);
    }

    // Sleep for the read time
    sim::SimUtilities::sleep(sim::Seconds(std::max(0.0, 1.0/sim::P()->frameRate() - duration)));

    // Return the value
    return value;
}

double MouseInterface::readGyro() {

    ENSURE_CONTINUOUS_INTERFACE

    return m_mouse->readGyro().getDegreesPerSecond();
}

bool MouseInterface::wallFront() {

    ENSURE_DISCRETE_INTERFACE

    return isWall(getDiscretizedTranslation(), getDiscretizedRotation());
}

bool MouseInterface::wallRight() {

    ENSURE_DISCRETE_INTERFACE

    std::pair<int, int> position = getDiscretizedTranslation();

    switch (getDiscretizedRotation()) {
        case Direction::NORTH:
            return isWall(position, Direction::EAST);
        case Direction::EAST:
            return isWall(position, Direction::SOUTH);
        case Direction::SOUTH:
            return isWall(position, Direction::WEST);
        case Direction::WEST:
            return isWall(position, Direction::NORTH);
    }
}

bool MouseInterface::wallLeft() {

    ENSURE_DISCRETE_INTERFACE

    std::pair<int, int> position = getDiscretizedTranslation();

    switch (getDiscretizedRotation()) {
        case Direction::NORTH:
            return isWall(position, Direction::WEST);
        case Direction::EAST:
            return isWall(position, Direction::NORTH);
        case Direction::SOUTH:
            return isWall(position, Direction::EAST);
        case Direction::WEST:
            return isWall(position, Direction::SOUTH);
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
    Degrees destinationRotation = m_mouse->getCurrentTranslationAndRotation().second;

    // A single step to take within the while loops
    auto step = [=](){
        m_mouse->setWheelSpeeds({
            {"left",  RevolutionsPerSecond(-P()->discreteInterfaceWheelSpeed())},
            {"right", RevolutionsPerSecond( P()->discreteInterfaceWheelSpeed())},
        });
        sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
    };

    switch (getDiscretizedRotation()) {
        case Direction::NORTH: {
            destinationTranslation += Cartesian(Meters(0), tileLength);
            while (m_mouse->getCurrentTranslationAndRotation().first.getY() < destinationTranslation.getY()) {
                step();
            }
            break;
        }
        case Direction::EAST: {
            destinationTranslation += Cartesian(tileLength, Meters(0));
            while (m_mouse->getCurrentTranslationAndRotation().first.getX() < destinationTranslation.getX()) {
                step();
            }
            break;
        }
        case Direction::SOUTH: {
            destinationTranslation += Cartesian(Meters(0), tileLength * -1);
            while (destinationTranslation.getY() < m_mouse->getCurrentTranslationAndRotation().first.getY()) {
                step();
            }
            break;
        }
        case Direction::WEST: {
            destinationTranslation += Cartesian(tileLength * -1, Meters(0));
            while (destinationTranslation.getX() < m_mouse->getCurrentTranslationAndRotation().first.getX()) {
                step();
            }
            break;
        }
    }

    m_mouse->setWheelSpeeds({
        {"left",  RevolutionsPerSecond(0)},
        {"right", RevolutionsPerSecond(0)},
    });
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::turnRight() {

    ENSURE_DISCRETE_INTERFACE

    std::pair<Cartesian, Radians> currentTranslationAndRotation = m_mouse->getCurrentTranslationAndRotation();
    Cartesian destinationTranslation = currentTranslationAndRotation.first;
    Degrees destinationRotation = currentTranslationAndRotation.second - Degrees(90);

    // A single step to take within the while loops
    auto step = [=](){
        m_mouse->setWheelSpeeds({
            {"left",  RevolutionsPerSecond(-P()->discreteInterfaceWheelSpeed()/2.0)},
            {"right", RevolutionsPerSecond(-P()->discreteInterfaceWheelSpeed()/2.0)},
        });
        sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
    };

    switch (getDiscretizedRotation()) {
        case Direction::NORTH: {
            while (m_mouse->getCurrentTranslationAndRotation().second < Degrees(180)) {
                step();
            }
            break;
        }
        case Direction::EAST: {
            while (m_mouse->getCurrentTranslationAndRotation().second < Degrees(180) ||
                    destinationRotation < m_mouse->getCurrentTranslationAndRotation().second) {
                step();
            }
            break;
        }
        case Direction::SOUTH:
        case Direction::WEST: {
            while (destinationRotation < m_mouse->getCurrentTranslationAndRotation().second) {
                step();
            }
            break;
        }
    }

    m_mouse->setWheelSpeeds({
        {"left",  RevolutionsPerSecond(0)},
        {"right", RevolutionsPerSecond(0)},
    });
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::turnLeft() {

    ENSURE_DISCRETE_INTERFACE

    std::pair<Cartesian, Radians> currentTranslationAndRotation = m_mouse->getCurrentTranslationAndRotation();
    Cartesian destinationTranslation = currentTranslationAndRotation.first;
    Degrees destinationRotation = currentTranslationAndRotation.second + Degrees(90);

    // A single step to take within the while loops
    auto step = [=](){
        m_mouse->setWheelSpeeds({
            {"left",  RevolutionsPerSecond(P()->discreteInterfaceWheelSpeed()/2.0)},
            {"right", RevolutionsPerSecond(P()->discreteInterfaceWheelSpeed()/2.0)},
        });
        sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
    };

    switch (getDiscretizedRotation()) {
        case Direction::EAST: {
            while (Degrees(180) < m_mouse->getCurrentTranslationAndRotation().second ||
                    m_mouse->getCurrentTranslationAndRotation().second < destinationRotation) {
                step();
            }
            break;
        }
        case Direction::SOUTH: {
            while (Degrees(180) < m_mouse->getCurrentTranslationAndRotation().second) {
                step();
            }
            break;
        }
        case Direction::WEST:
        case Direction::NORTH: {
            while (m_mouse->getCurrentTranslationAndRotation().second < destinationRotation) {
                step();
            }
            break;
        }
    }

    m_mouse->setWheelSpeeds({
        {"left",  RevolutionsPerSecond(0)},
        {"right", RevolutionsPerSecond(0)},
    });
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::turnAround() {

    ENSURE_DISCRETE_INTERFACE

    turnRight();
    turnRight();
}

void MouseInterface::ensureDiscreteInterface(const std::string& callingFunction) const {
    if (S()->interfaceType() != InterfaceType::DISCRETE) {
        L()->error(
            "You must declare the interface type to be"
            " sim::InterfaceType::DISCRETE to use MouseInterface::%v().",
            callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureContinuousInterface(const std::string& callingFunction) const {
    if (S()->interfaceType() != InterfaceType::CONTINUOUS) {
        L()->error(
            "You must declare the interface type to be sim::InterfaceType::CONTINUOUS to use MouseInterface::%v().",
            callingFunction);
        SimUtilities::quit();
    }
}

bool MouseInterface::isWall(std::pair<int, int> position, Direction direction) {

    ASSERT_TR(m_maze->withinMaze(position.first, position.second));

    bool wallExists = m_maze->getTile(position.first, position.second)->isWall(direction);

    if (P()->discreteInterfaceDeclareWallOnRead()) {
        declareWall(position.first, position.second, DIRECTION_TO_CHAR.at(direction), wallExists);
    }

    return wallExists;
}

bool MouseInterface::hasOpposingWall(int x, int y, Direction direction) const {
    switch (direction) {
        case Direction::NORTH:
            return y < m_maze->getHeight() - 1;
        case Direction::EAST:
            return x < m_maze->getWidth() - 1;
        case Direction::SOUTH:
            return y > 0;
        case Direction::WEST:
            return x > 0;
    }
}

std::pair<std::pair<int, int>, Direction> MouseInterface::getOpposingWall(int x, int y, Direction direction) const {
    ASSERT_TR(hasOpposingWall(x, y, direction));
    switch (direction) {
        case Direction::NORTH:
            return std::make_pair(std::make_pair(x, y + 1), Direction::SOUTH);
        case Direction::EAST:
            return std::make_pair(std::make_pair(x + 1, y), Direction::WEST);
        case Direction::SOUTH:
            return std::make_pair(std::make_pair(x, y - 1), Direction::NORTH);
        case Direction::WEST:
            return std::make_pair(std::make_pair(x - 1, y), Direction::EAST);
    }
}

std::pair<int, int> MouseInterface::getDiscretizedTranslation() const {
    return m_mouse->getDiscretizedTranslation();
}

Direction MouseInterface::getDiscretizedRotation() const {
    return m_mouse->getDiscretizedRotation();
}

} // namespace sim

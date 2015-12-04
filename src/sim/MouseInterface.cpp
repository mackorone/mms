#include "MouseInterface.h"

#include "units/Meters.h"
#include "units/MetersPerSecond.h"
#include "units/Milliseconds.h"
#include "units/RevolutionsPerMinute.h"
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

    if (!P()->algorithmControlsTileFog()) { 
        L()->warn(
            "The simulation parameters indicate that the simulator should"
            " control the tile fog, not the algorithm. Thus you may not set"
            " the fogginess for tile in position (%v, %v).", x, y);
        return;
    }

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and thus you cannot set"
            " its fogginess.", x, y);
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

double MouseInterface::getWheelMaxSpeed(const std::string& name) {

    ENSURE_CONTINUOUS_INTERFACE

    if (!m_mouse->hasWheel(name)) {
        L()->warn("There is no wheel called \"%v\" and thus you cannot get its max speed.", name);
        return 0.0;
    }

    return m_mouse->getWheelMaxSpeed(name).getRevolutionsPerMinute();
}

void MouseInterface::setWheelSpeed(const std::string& name, double rpm) {

    ENSURE_CONTINUOUS_INTERFACE

    if (!m_mouse->hasWheel(name)) {
        L()->warn("There is no wheel called \"%v\" and thus you cannot set its speed.", name);
        return;
    }

    if (getWheelMaxSpeed(name) < std::abs(rpm)) {
        L()->warn(
            "You're attempting to set the speed of wheel \"%v\" to %v rpm,"
            " which has magnitude greater than the max speed of %v rpm. Thus,"
            " the wheel speed was not set.", name, rpm, getWheelMaxSpeed(name));
        return;
    }

    m_mouse->setWheelSpeeds({{name, RadiansPerSecond(RevolutionsPerMinute(rpm))}});
}

double MouseInterface::getWheelEncoderTicksPerRevolution(const std::string& name) {

    ENSURE_CONTINUOUS_INTERFACE

    if (!m_mouse->hasWheel(name)) {
        L()->warn(
            "There is no wheel called \"%v\" and thus you cannot get its number"
            " of encoder ticks per revolution.", name);
        return 0.0;
    }

    return m_mouse->getWheelEncoderTicksPerRevolution(name);
}

int MouseInterface::readWheelEncoder(const std::string& name) {

    ENSURE_CONTINUOUS_INTERFACE

    if (!m_mouse->hasWheel(name)) {
        L()->warn("There is no wheel called \"%v\" and thus you cannot read its encoder.", name);
        return 0;
    }

    return m_mouse->readWheelEncoder(name);
}

double MouseInterface::readSensor(std::string name) {

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
            (duration - readDurationSeconds) / readDurationSeconds * 100);
    }

    // Sleep for the read time
    sim::SimUtilities::sleep(sim::Seconds(std::max(0.0, 1.0 / sim::P()->frameRate() - duration)));

    // Return the value
    return value;
}

double MouseInterface::readGyro() {

    ENSURE_CONTINUOUS_INTERFACE

    return m_mouse->readGyro().getDegreesPerSecond();
}

bool MouseInterface::wallFront() {

    ENSURE_DISCRETE_INTERFACE

    return isWall(getCurrentDiscretizedTranslation(), getCurrentDiscretizedRotation());
}

bool MouseInterface::wallRight() {

    ENSURE_DISCRETE_INTERFACE

    std::pair<int, int> position = getCurrentDiscretizedTranslation();

    switch (getCurrentDiscretizedRotation()) {
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

    std::pair<int, int> position = getCurrentDiscretizedTranslation();

    switch (getCurrentDiscretizedRotation()) {
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

    // Get the length of a single tile
    Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());

    // We modify these values in the switch statement
    Cartesian destinationTranslation = m_mouse->getCurrentTranslation();
    Degrees destinationRotation = m_mouse->getCurrentRotation();

    m_mouse->setWheelSpeedsForMoveForward();

    switch (getCurrentDiscretizedRotation()) {
        case Direction::NORTH: {
            destinationTranslation += Cartesian(Meters(0), tileLength);
            while (m_mouse->getCurrentTranslation().getY() < destinationTranslation.getY()) {
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
        case Direction::EAST: {
            destinationTranslation += Cartesian(tileLength, Meters(0));
            while (m_mouse->getCurrentTranslation().getX() < destinationTranslation.getX()) {
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
        case Direction::SOUTH: {
            destinationTranslation += Cartesian(Meters(0), tileLength * -1);
            while (destinationTranslation.getY() < m_mouse->getCurrentTranslation().getY()) {
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
        case Direction::WEST: {
            destinationTranslation += Cartesian(tileLength * -1, Meters(0));
            while (destinationTranslation.getX() < m_mouse->getCurrentTranslation().getX()) {
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
    }

    m_mouse->stopAllWheels();
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::turnLeft() {

    ENSURE_DISCRETE_INTERFACE

    Cartesian destinationTranslation = m_mouse->getCurrentTranslation();
    Degrees destinationRotation = m_mouse->getCurrentRotation() + Degrees(90);

    m_mouse->setWheelSpeedsForTurnLeft();

    switch (getCurrentDiscretizedRotation()) {
        case Direction::EAST: {
            while (Degrees(180) < m_mouse->getCurrentRotation() ||
                    m_mouse->getCurrentRotation() < destinationRotation) {
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
        case Direction::SOUTH: {
            while (Degrees(180) < m_mouse->getCurrentRotation()) {
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
        case Direction::WEST:
        case Direction::NORTH: {
            while (m_mouse->getCurrentRotation() < destinationRotation) {
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
    }

    m_mouse->stopAllWheels();
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::turnRight() {

    ENSURE_DISCRETE_INTERFACE

    Cartesian destinationTranslation = m_mouse->getCurrentTranslation();
    Degrees destinationRotation = m_mouse->getCurrentRotation() - Degrees(90);

    m_mouse->setWheelSpeedsForTurnRight();

    switch (getCurrentDiscretizedRotation()) {
        case Direction::NORTH: {
            while (m_mouse->getCurrentRotation() < Degrees(180)) {
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
        case Direction::EAST: {
            while (m_mouse->getCurrentRotation() < Degrees(180) ||
                    destinationRotation < m_mouse->getCurrentRotation()) {
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
        case Direction::SOUTH:
        case Direction::WEST: {
            while (destinationRotation < m_mouse->getCurrentRotation()) {
                sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            }
            break;
        }
    }

    m_mouse->stopAllWheels();
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
            "You must declare the interface type to be \"%v\" to use MouseInterface::%v().",
            INTERFACE_TYPE_TO_STRING.at(InterfaceType::DISCRETE), callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureContinuousInterface(const std::string& callingFunction) const {
    if (S()->interfaceType() != InterfaceType::CONTINUOUS) {
        L()->error(
            "You must declare the interface type to be \"%v\" to use MouseInterface::%v().",
            INTERFACE_TYPE_TO_STRING.at(InterfaceType::CONTINUOUS), callingFunction);
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

std::pair<int, int> MouseInterface::getCurrentDiscretizedTranslation() const {
    return m_mouse->getCurrentDiscretizedTranslation();
}

Direction MouseInterface::getCurrentDiscretizedRotation() const {
    return m_mouse->getCurrentDiscretizedRotation();
}

} // namespace sim

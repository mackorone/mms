#include "MouseInterface.h"

#include "ContainerUtilities.h"
#include "InterfaceType.h"
#include "Logging.h"
#include "Param.h"
#include "SimUtilities.h"

namespace sim {

MouseInterface::MouseInterface(
        const Maze* maze,
        Mouse* mouse,
        MazeGraphic* mazeGraphic,
        MouseInterfaceOptions options,
        std::set<char> allowableTileTextCharacters) :
        m_maze(maze),
        m_mouse(mouse),
        m_impl(maze, mouse, mazeGraphic),
        m_options(options),
        m_allowableTileTextCharacters(allowableTileTextCharacters),
        m_inOrigin(true) {
}

void MouseInterface::debug(const std::string& str) {
    m_impl.debug(str);
}

void MouseInterface::info(const std::string& str) {
    m_impl.info(str);
}

void MouseInterface::warn(const std::string& str) {
    m_impl.warn(str);
}

void MouseInterface::error(const std::string& str) {
    m_impl.error(str);
}

double MouseInterface::getRandom() {
    return m_impl.getRandom();
}

int MouseInterface::millis() {
    return m_impl.millis();
}

void MouseInterface::delay(int milliseconds) {
    m_impl.delay(milliseconds);
}

void MouseInterface::quit() {
    m_impl.quit();
}

void MouseInterface::setTileColor(int x, int y, char color) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v) and"
            " thus you cannot set its color.", x, y);
        return;
    }

    if (!ContainerUtilities::mapContains(CHAR_TO_COLOR, color)) {
        L()->warn(
            "You cannot set the color of tile (%v, %v) to"
            " '%v' since '%v' is not mapped to a color.",
            x, y, color, color);
        return;
    }

    m_impl.setTileColor(x, y, color);
}

void MouseInterface::clearTileColor(int x, int y) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and"
            " thus you cannot clear its color.", x, y);
        return;
    }

    m_impl.clearTileColor(x, y);
}

void MouseInterface::clearAllTileColor() {
    m_impl.clearAllTileColor();
}

void MouseInterface::setTileText(int x, int y, std::string text) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and thus"
            " you cannot set its text to \"%v\".", x, y, text);
        return;
    }

    for (int i = 0; i < text.size(); i += 1) {
        char c = text.at(i);
        if (!ContainerUtilities::setContains(m_allowableTileTextCharacters, c)) {
            L()->warn(
                "Unable to set the tile text for unprintable character \"%v\"."
                " Using the character \"%v\" instead.",
                (c == '\n' ? "\\n" :
                (c == '\t' ? "\\t" :
                (c == '\r' ? "\\r" :
                std::to_string(c)))),
                P()->defaultTileTextCharacter());
            text[i] = P()->defaultTileTextCharacter();
        }
    }

    m_impl.setTileText(x, y, text, m_options.tileTextNumberOfRows, m_options.tileTextNumberOfCols);
}

void MouseInterface::clearTileText(int x, int y) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and"
            " thus you cannot clear its text.", x, y);
        return;
    }

    m_impl.clearTileText(x, y);
}

void MouseInterface::clearAllTileText() {
    m_impl.clearAllTileText();
}

void MouseInterface::declareWall(int x, int y, char direction, bool wallExists) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and thus"
            " you cannot declare any of its walls.", x, y);
        return;
    }

    if (!ContainerUtilities::mapContains(CHAR_TO_DIRECTION, direction)) {
        L()->warn("The character '%v' is not mapped to a valid direction.", direction);
        return;
    }

    m_impl.declareWall(
        std::make_pair(
            std::make_pair(x, y),
            CHAR_TO_DIRECTION.at(direction)
        ),
        wallExists,
        m_options.declareBothWallHalves
    );
}

void MouseInterface::undeclareWall(int x, int y, char direction) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v) and thus"
            " you cannot undeclare any of its walls.", x, y);
        return;
    }

    if (!ContainerUtilities::mapContains(CHAR_TO_DIRECTION, direction)) {
        L()->warn("The character '%v' is not mapped to a valid direction.", direction);
        return;
    }

    m_impl.undeclareWall(
        std::make_pair(
            std::make_pair(x, y),
            CHAR_TO_DIRECTION.at(direction)
        ),
        m_options.declareBothWallHalves
    );
}

void MouseInterface::setTileFogginess(int x, int y, bool foggy) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and"
            " thus you cannot set its fogginess.", x, y);
        return;
    }

    m_impl.setTileFogginess(x, y, foggy);
}

void MouseInterface::declareTileDistance(int x, int y, int distance) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and"
            " thus you cannot set its distance.", x, y);
        return;
    }

    m_impl.declareTileDistance(
        x, y, distance,
        m_options.setTileTextWhenDistanceDeclared,
        m_options.setTileBaseColorWhenDistanceDeclaredCorrectly,
        m_options.tileTextNumberOfRows,
        m_options.tileTextNumberOfCols
    );
}

void MouseInterface::undeclareTileDistance(int x, int y) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and"
            " thus you cannot clear its distance.", x, y);
        return;
    }

    m_impl.undeclareTileDistance(
        x, y,
        m_options.setTileTextWhenDistanceDeclared,
        m_options.setTileBaseColorWhenDistanceDeclaredCorrectly
    );
}

void MouseInterface::resetPosition() {
    m_impl.resetPosition();
}

bool MouseInterface::inputButtonPressed(int inputButton) {

    if (inputButton < 0 || 9 < inputButton) {
        L()->warn(
            "There is no input button with the number %v, and thus you cannot"
            " check to see if it has been pressed.", inputButton);
        return false;
    }

    return m_impl.inputButtonPressed(inputButton);
}

void MouseInterface::acknowledgeInputButtonPressed(int inputButton) {

    if (inputButton < 0 || 9 < inputButton) {
        L()->warn(
            "There is no input button with the number %v, and thus you cannot"
            " acknowledge that it has been pressed.", inputButton);
        return;
    }

    m_impl.acknowledgeInputButtonPressed(inputButton);
    L()->info("Input button %v was acknowledged as pressed; it can now be pressed again.", inputButton);
}

double MouseInterface::getWheelMaxSpeed(const std::string& name) {

    ENSURE_CONTINUOUS_INTERFACE

    if (!m_mouse->hasWheel(name)) {
        L()->warn("There is no wheel called \"%v\" and thus you cannot get its max speed.", name);
        return 0.0;
    }

    return m_impl.getWheelMaxSpeed(name);
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

    m_impl.setWheelSpeed(name, rpm);
}

double MouseInterface::getWheelEncoderTicksPerRevolution(const std::string& name) {

    ENSURE_CONTINUOUS_INTERFACE

    if (!m_mouse->hasWheel(name)) {
        L()->warn(
            "There is no wheel called \"%v\" and thus you cannot get its number"
            " of encoder ticks per revolution.", name);
        return 0.0;
    }

    return m_impl.getWheelEncoderTicksPerRevolution(name);
}

int MouseInterface::readWheelEncoder(const std::string& name) {

    ENSURE_CONTINUOUS_INTERFACE

    if (!m_mouse->hasWheel(name)) {
        L()->warn("There is no wheel called \"%v\" and thus you cannot read its encoder.", name);
        return 0;
    }

    return m_impl.readWheelEncoder(name);
}

void MouseInterface::resetWheelEncoder(const std::string& name) {

    ENSURE_CONTINUOUS_INTERFACE

    if (!m_mouse->hasWheel(name)) {
        L()->warn("There is no wheel called \"%v\" and thus you cannot reset its encoder.", name);
        return;
    }

    if (m_mouse->getWheelEncoderType(name) != EncoderType::RELATIVE) {
        L()->warn(
            "The encoder type of the wheel \"%v\" is \"%v\". However, you may"
            " only reset the wheel encoder if the encoder type is \"%v\".",
            name,
            ENCODER_TYPE_TO_STRING.at(m_mouse->getWheelEncoderType(name)),
            ENCODER_TYPE_TO_STRING.at(EncoderType::RELATIVE));
        return;
    }

    m_impl.resetWheelEncoder(name);
}

double MouseInterface::readSensor(std::string name) {

    ENSURE_CONTINUOUS_INTERFACE

    if (!m_mouse->hasSensor(name)) {
        L()->warn("There is no sensor called \"%v\" and thus you cannot read its value.", name);
        return 0.0;
    }

    return m_impl.readSensor(name);
}

double MouseInterface::readGyro() {

    ENSURE_CONTINUOUS_INTERFACE

    return m_impl.readGyro();
}

bool MouseInterface::wallFront() {

    ENSURE_DISCRETE_INTERFACE

    return m_impl.wallFront(m_options.declareWallOnRead, m_options.declareBothWallHalves);
}

bool MouseInterface::wallRight() {

    ENSURE_DISCRETE_INTERFACE

    return m_impl.wallRight(m_options.declareWallOnRead, m_options.declareBothWallHalves);
}

bool MouseInterface::wallLeft() {

    ENSURE_DISCRETE_INTERFACE

    return m_impl.wallLeft(m_options.declareWallOnRead, m_options.declareBothWallHalves);
}

void MouseInterface::moveForward() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    m_impl.moveForward(m_options.wheelSpeedFraction);
}

void MouseInterface::moveForward(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    m_impl.moveForward(count, m_options.wheelSpeedFraction);
}

void MouseInterface::turnLeft() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    m_impl.turnLeft(m_options.wheelSpeedFraction);
}

void MouseInterface::turnRight() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    m_impl.turnRight(m_options.wheelSpeedFraction);
}

void MouseInterface::turnAroundLeft() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    m_impl.turnAroundLeft(m_options.wheelSpeedFraction);
}

void MouseInterface::turnAroundRight() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    m_impl.turnAroundRight(m_options.wheelSpeedFraction);
}

void MouseInterface::originMoveForwardToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_INSIDE_ORIGIN

    m_impl.originMoveForwardToEdge(m_options.wheelSpeedFraction);
    m_inOrigin = false;
}

void MouseInterface::originTurnLeftInPlace() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_INSIDE_ORIGIN

    m_impl.originTurnLeftInPlace(m_options.wheelSpeedFraction);
}

void MouseInterface::originTurnRightInPlace() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_INSIDE_ORIGIN

    m_impl.originTurnRightInPlace(m_options.wheelSpeedFraction);
}

void MouseInterface::moveForwardToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    m_impl.moveForwardToEdge(m_options.wheelSpeedFraction);
}

void MouseInterface::moveForwardToEdge(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    m_impl.moveForwardToEdge(count, m_options.wheelSpeedFraction);
}

void MouseInterface::turnLeftToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    m_impl.turnLeftToEdge(m_options.wheelSpeedFraction);
}

void MouseInterface::turnRightToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    m_impl.turnRightToEdge(m_options.wheelSpeedFraction);
}

void MouseInterface::turnAroundLeftToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    m_impl.turnAroundLeftToEdge(m_options.wheelSpeedFraction);
}

void MouseInterface::turnAroundRightToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    m_impl.turnAroundRightToEdge(m_options.wheelSpeedFraction);
}

void MouseInterface::diagonalLeftLeft(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    m_impl.diagonalLeftLeft(count, m_options.wheelSpeedFraction);
}

void MouseInterface::diagonalLeftRight(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    m_impl.diagonalLeftRight(count, m_options.wheelSpeedFraction);
}

void MouseInterface::diagonalRightLeft(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    m_impl.diagonalRightLeft(count, m_options.wheelSpeedFraction);
}

void MouseInterface::diagonalRightRight(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    m_impl.diagonalRightRight(count, m_options.wheelSpeedFraction);
}

int MouseInterface::currentXTile() {

    ENSURE_ALLOW_OMNISCIENCE

    return m_impl.currentXTile();
}

int MouseInterface::currentYTile() {

    ENSURE_ALLOW_OMNISCIENCE

    return m_impl.currentYTile();
}

char MouseInterface::currentDirection() {

    ENSURE_ALLOW_OMNISCIENCE

    return m_impl.currentDirection();
}

double MouseInterface::currentXPosMeters() {

    ENSURE_ALLOW_OMNISCIENCE

    return m_impl.currentXPosMeters();
}

double MouseInterface::currentYPosMeters() {

    ENSURE_ALLOW_OMNISCIENCE

    return m_impl.currentYPosMeters();
}

double MouseInterface::currentRotationDegrees() {

    ENSURE_ALLOW_OMNISCIENCE

    return m_impl.currentRotationDegrees();
}

void MouseInterface::ensureDiscreteInterface(const std::string& callingFunction) const {
    if (m_options.interfaceType != InterfaceType::DISCRETE) {
        L()->error(
            "You must declare the interface type to be \"%v\" to use \"MouseInterface::%v()\".",
            INTERFACE_TYPE_TO_STRING.at(InterfaceType::DISCRETE), callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureContinuousInterface(const std::string& callingFunction) const {
    if (m_options.interfaceType != InterfaceType::CONTINUOUS) {
        L()->error(
            "You must declare the interface type to be \"%v\" to use \"MouseInterface::%v()\".",
            INTERFACE_TYPE_TO_STRING.at(InterfaceType::CONTINUOUS), callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureAllowOmniscience(const std::string& callingFunction) const {
    if (!m_options.allowOmniscience) {
        L()->error(
            "You must return true from \"allowOmniscience()\" in order to use \"MouseInterface::%v()\".",
            callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureNotTileEdgeMovements(const std::string& callingFunction) const {
    if (m_options.useTileEdgeMovements) {
        L()->error(
            "You must return false from \"useTileEdgeMovements()\" in order to use \"MouseInterface::%v()\".",
            callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureUseTileEdgeMovements(const std::string& callingFunction) const {
    if (!m_options.useTileEdgeMovements) {
        L()->error(
            "You must return true from \"useTileEdgeMovements()\" in order to use \"MouseInterface::%v()\".",
            callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureInsideOrigin(const std::string& callingFunction) const {
    if (!m_inOrigin) {
        L()->error(
            "You should only call \"MouseInterface::%v()\" if you're in"
            " the origin, i.e., you haven't moved forward at all yet.",
            callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureOutsideOrigin(const std::string& callingFunction) const {
    if (m_inOrigin) {
        L()->error(
            "You should only call \"MouseInterface::%v()\" if you're"
            " outside of the origin, i.e., you've already called"
            " \"MouseInterface::originMoveForwardToEdge()\".",
            callingFunction);
        SimUtilities::quit();
    }
}

} // namespace sim

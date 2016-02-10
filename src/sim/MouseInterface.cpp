#include "MouseInterface.h"

#include "units/Meters.h"
#include "units/MetersPerSecond.h"
#include "units/Milliseconds.h"
#include "units/Polar.h"
#include "units/RevolutionsPerMinute.h"
#include "units/Seconds.h"

#include "Assert.h"
#include "Color.h"
#include "ContainerUtilities.h"
#include "CPMath.h"
#include "Logging.h"
#include "Param.h"
#include "State.h"
#include "SimUtilities.h"

namespace sim {

MouseInterface::MouseInterface(
        const Maze* maze,
        Mouse* mouse,
        MazeGraphic* mazeGraphic,
        std::set<char> allowableTileTextCharacters, 
        MouseInterfaceOptions options) :
        m_maze(maze),
        m_mouse(mouse),
        m_mazeGraphic(mazeGraphic),
        m_allowableTileTextCharacters(allowableTileTextCharacters),
        m_options(options) {
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

int MouseInterface::millis() {
    return m_mouse->getElapsedSimTime().getMilliseconds();
}

void MouseInterface::delay(int milliseconds) {
    sim::SimUtilities::sleep(Milliseconds(milliseconds / S()->simSpeed()));
}

void MouseInterface::quit() {
    sim::SimUtilities::quit();
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

    m_mazeGraphic->setTileColor(x, y, CHAR_TO_COLOR.at(color));
    m_tilesWithColor.insert(std::make_pair(x, y));
}

void MouseInterface::clearTileColor(int x, int y) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and"
            " thus you cannot clear its color.", x, y);
        return;
    }

    m_mazeGraphic->setTileColor(x, y, STRING_TO_COLOR.at(P()->tileBaseColor()));
    m_tilesWithColor.erase(std::make_pair(x, y));
}

void MouseInterface::clearAllTileColor() {
    for (std::pair<int, int> position : m_tilesWithColor) {
        clearTileColor(position.first, position.second);
    }
}

void MouseInterface::setTileText(int x, int y, const std::string& text) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and thus"
            " you cannot set its text to \"%v\".", x, y, text);
        return;
    }

    // TODO: MACK - clean this up
    std::vector<std::string> rowsOfText;
    int row = 0;
    int index = 0;
    while (row < m_options.tileTextNumberOfRows && index < text.size()) {
        std::string rowOfText;
        while (index < (row + 1) * m_options.tileTextNumberOfCols && index < text.size()) {
            char c = text.at(index);
            if (!ContainerUtilities::setContains(m_allowableTileTextCharacters, c)) {
                L()->warn(
                    "Unable to set the tile text for unprintable character \"%v\"."
                    " Using the character \"%v\" instead.",
                    (c == '\n' ? "\\n" :
                    (c == '\t' ? "\\t" :
                    (c == '\r' ? "\\r" :
                    std::to_string(c)))),
                    P()->defaultTileTextCharacter());
                c = P()->defaultTileTextCharacter();
            }
            rowOfText += c;
            index += 1;
        }
        rowsOfText.push_back(rowOfText); 
        row += 1;
    }
    m_mazeGraphic->setTileText(x, y, rowsOfText);
    m_tilesWithText.insert(std::make_pair(x, y));
}

void MouseInterface::clearTileText(int x, int y) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and"
            " thus you cannot clear its text.", x, y);
        return;
    }

    m_mazeGraphic->setTileText(x, y, {});
    m_tilesWithText.erase(std::make_pair(x, y));
}

void MouseInterface::clearAllTileText() {
    for (std::pair<int, int> position : m_tilesWithText) {
        clearTileText(position.first, position.second);
    }
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

    declareWallImpl(
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

    undeclareWallImpl(
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

    m_mazeGraphic->setTileFogginess(x, y, foggy);
}

void MouseInterface::declareTileDistance(int x, int y, int distance) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and"
            " thus you cannot set its distance.", x, y);
        return;
    }

    if (m_options.setTileTextWhenDistanceDeclared) {
        setTileText(x, y, (0 <= distance ? std::to_string(distance) : "inf"));
    }

    if (m_options.setTileBaseColorWhenDistanceDeclaredCorrectly) {
        if (distance == m_maze->getTile(x, y)->getDistance()) {
            setTileColor(x, y,
                COLOR_TO_CHAR.at(STRING_TO_COLOR.at(P()->distanceCorrectTileBaseColor())));
        }
    }
}

void MouseInterface::undeclareTileDistance(int x, int y) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and"
            " thus you cannot clear its distance.", x, y);
        return;
    }

    if (m_options.setTileTextWhenDistanceDeclared) {
        setTileText(x, y, "");
    }

    if (m_options.setTileBaseColorWhenDistanceDeclaredCorrectly) {
        setTileColor(x, y, COLOR_TO_CHAR.at(STRING_TO_COLOR.at(P()->tileBaseColor())));
    }
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

    m_mouse->setWheelSpeeds({{name, RevolutionsPerMinute(rpm)}});
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

    switch (m_mouse->getWheelEncoderType(name)) {
        case EncoderType::ABSOLUTE:
            return m_mouse->readWheelAbsoluteEncoder(name);
        case EncoderType::RELATIVE:
            return m_mouse->readWheelRelativeEncoder(name);
    }
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

    m_mouse->resetWheelRelativeEncoder(name);
}

double MouseInterface::readSensor(std::string name) {

    ENSURE_CONTINUOUS_INTERFACE

    if (!m_mouse->hasSensor(name)) {
        L()->warn("There is no sensor called \"%v\" and thus you cannot read its value.", name);
        return 0.0;
    }

    // TODO: MACK - test this
    //sim::SimUtilities::sleep(m_mouse->getSensorReadDuration(name));
    return m_mouse->readSensor(name);
}

double MouseInterface::readGyro() {

    ENSURE_CONTINUOUS_INTERFACE

    return m_mouse->readGyro().getDegreesPerSecond();
}

bool MouseInterface::wallFront() {

    ENSURE_DISCRETE_INTERFACE

    return wallFrontImpl(m_options.declareWallOnRead, m_options.declareBothWallHalves);
}

bool MouseInterface::wallRight() {

    ENSURE_DISCRETE_INTERFACE

    return wallRightImpl(m_options.declareWallOnRead, m_options.declareBothWallHalves);
}

bool MouseInterface::wallLeft() {

    ENSURE_DISCRETE_INTERFACE

    return wallLeftImpl(m_options.declareWallOnRead, m_options.declareBothWallHalves);
}

void MouseInterface::moveForward() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    if (wallFrontImpl(false, false)) {
        if (!S()->crashed()) {
            S()->setCrashed();
        }
        return;
    }

    // TODO: MACK - make this better
    moveForwardTo(getDestinationTranslationForMoveForward(), m_mouse->getCurrentRotation());
}

void MouseInterface::moveForward(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    for (int i = 0; i < count; i += 1) {
        moveForward();
    }
}

void MouseInterface::turnLeft() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    turnTo(m_mouse->getCurrentTranslation(), m_mouse->getCurrentRotation() + Degrees(90));
}

void MouseInterface::turnRight() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    turnTo(m_mouse->getCurrentTranslation(), m_mouse->getCurrentRotation() - Degrees(90));
}

void MouseInterface::turnAroundLeft() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    for (int i = 0; i < 2; i += 1) {
        turnLeft();
    }
}

void MouseInterface::turnAroundRight() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    for (int i = 0; i < 2; i += 1) {
        turnRight();
    }
}

void MouseInterface::originMoveForwardToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS

    // TODO: MACK

}

void MouseInterface::originTurnLeftInPlace() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS

    // TODO: MACK

}

void MouseInterface::originTurnRightInPlace() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS

    // TODO: MACK
}

void MouseInterface::moveForwardToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS

    // TODO: MACK Deduplicate here...
    if (wallFrontImpl(false, false)) {
        if (!S()->crashed()) {
            S()->setCrashed();
        }
        return;
    }

    moveForwardTo(getDestinationTranslationForMoveForward(), m_mouse->getCurrentRotation());
}

void MouseInterface::moveForwardToEdge(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS

    for (int i = 0; i < count; i += 1) {
        moveForwardToEdge();
    }
}

void MouseInterface::turnLeftToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS

    // TODO: MACK - check for crashes

    // TODO: MACK
    Degrees destinationRotation = m_mouse->getCurrentRotation() + Degrees(90);

    // TODO: MACK - make special methods???
    Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    Meters halfWallWidth = Meters(P()->wallWidth() / 2.0);
    Cartesian destinationTranslation = Cartesian(
        tileLength * m_mouse->getCurrentDiscretizedTranslation().first,
        tileLength * m_mouse->getCurrentDiscretizedTranslation().second
    );

    switch (m_mouse->getCurrentDiscretizedRotation()) {
        case Direction::NORTH: {
            destinationTranslation += Cartesian(halfWallWidth * -1, tileLength / 2.0);
            break;
        }
        case Direction::EAST: {
            destinationTranslation += Cartesian(tileLength / 2.0, tileLength + halfWallWidth);
            break;
        }
        case Direction::SOUTH: {
            destinationTranslation += Cartesian(tileLength + halfWallWidth, tileLength / 2.0);
            break;
        }
        case Direction::WEST: {
            destinationTranslation += Cartesian(tileLength / 2.0, halfWallWidth * -1);
            break;
        }
    }

    // TODO: MACK - destination translation is incorrect here
    arcTo(destinationTranslation, destinationRotation, Meters(P()->wallLength() / 2.0));
    moveForwardTo(destinationTranslation, destinationRotation);
}

void MouseInterface::turnRightToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS

    // TODO: MACK - check for crashes

    // TODO: MACK
    Degrees destinationRotation = m_mouse->getCurrentRotation() - Degrees(90);

    // TODO: MACK - make special methods???
    Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    Meters halfWallWidth = Meters(P()->wallWidth() / 2.0);
    Cartesian destinationTranslation = Cartesian(
        tileLength * m_mouse->getCurrentDiscretizedTranslation().first,
        tileLength * m_mouse->getCurrentDiscretizedTranslation().second
    );

    switch (m_mouse->getCurrentDiscretizedRotation()) {
        case Direction::NORTH: {
            destinationTranslation += Cartesian(tileLength + halfWallWidth, tileLength / 2.0);
            break;
        }
        case Direction::EAST: {
            destinationTranslation += Cartesian(tileLength / 2.0, halfWallWidth * -1);
            break;
        }
        case Direction::SOUTH: {
            destinationTranslation += Cartesian(halfWallWidth * -1, tileLength / 2.0);
            break;
        }
        case Direction::WEST: {
            destinationTranslation += Cartesian(tileLength / 2.0, tileLength + halfWallWidth);
            break;
        }
    }

    // TODO: MACK - destination translation is incorrect here
    arcTo(destinationTranslation, destinationRotation, Meters(P()->wallLength() / 2.0));
    moveForwardTo(destinationTranslation, destinationRotation);
}

void MouseInterface::turnAroundLeftToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS

    Cartesian dest = m_mouse->getCurrentTranslation() + Polar(Meters(P()->wallLength() / 2.0), m_mouse->getCurrentRotation());
    moveForwardTo(dest, m_mouse->getCurrentRotation());

    for (int i = 0; i < 2; i += 1) {
        // TODO: MACK - separate out the implementation here, don't call the user method here
        turnLeft();
    }


    // TODO: MACK - originMoveForward?
    moveForwardToEdge();
}

void MouseInterface::turnAroundRightToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS

    Cartesian dest = m_mouse->getCurrentTranslation() + Polar(Meters(P()->wallLength() / 2.0), m_mouse->getCurrentRotation());
    moveForwardTo(dest, m_mouse->getCurrentRotation());

    for (int i = 0; i < 2; i += 1) {
        turnRight();
    }

    // TODO: MACK - originMoveForward?
    moveForwardToEdge();
}

void MouseInterface::diagonalLeftLeft(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS

    doDiagonal(count, true, true);
}

void MouseInterface::diagonalLeftRight(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS

    doDiagonal(count, true, false);
}

void MouseInterface::diagonalRightLeft(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS

    doDiagonal(count, false, true);
}

void MouseInterface::diagonalRightRight(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS

    doDiagonal(count, false, false);
}

void MouseInterface::doDiagonal(int count, bool startLeft, bool endLeft) {
    // TODO: MACK - special case for counts 1,2
    // TODO: MACK - limits on count?
    if (startLeft == endLeft) {
        ASSERT_EQ(count % 2, 1);
    }
    else {
        ASSERT_EQ(count % 2, 0);
    }
    // TODO: MACK - Clean this up

    Meters halfTileWidth = Meters(P()->wallLength() + P()->wallWidth()) / 2.0;
    Meters halfTileDiagonal = Meters(std::sqrt(2 * (halfTileWidth * halfTileWidth).getMetersSquared()));

    Cartesian backALittleBit = m_mouse->getCurrentTranslation() +
        Polar(Meters(P()->wallWidth() / 2.0), m_mouse->getCurrentRotation() + Degrees(180));

    Cartesian destination = backALittleBit +
        Polar(halfTileDiagonal * count, m_mouse->getCurrentRotation() + Degrees(45) * (startLeft ? 1 : -1));
    Polar delta = destination - m_mouse->getCurrentTranslation();

    Radians endRotation = m_mouse->getCurrentRotation();
    if (startLeft && endLeft) {
        endRotation += Degrees(90);
    }
    if (!startLeft && !endLeft) {
        endRotation -= Degrees(90);
    }
    
    turnTo(m_mouse->getCurrentTranslation(), delta.getTheta());
    moveForwardTo(destination, m_mouse->getCurrentRotation());
    turnTo(m_mouse->getCurrentTranslation(), endRotation);
    moveForwardTo(destination + Polar(Meters(P()->wallWidth() / 2.0), m_mouse->getCurrentRotation()), m_mouse->getCurrentRotation());

    // TODO: MACK - negative here?
    //arcTo(m_mouse->getCurrentTranslation(), delta.getTheta() * -2, Meters(P()->wallLength() / 4));
    // TODO: MACK
}

int MouseInterface::currentXTile() {

    ENSURE_ALLOW_OMNISCIENCE

    return m_mouse->getCurrentDiscretizedTranslation().first;
}

int MouseInterface::currentYTile() {

    ENSURE_ALLOW_OMNISCIENCE

    return m_mouse->getCurrentDiscretizedTranslation().second;
}

char MouseInterface::currentDirection() {

    ENSURE_ALLOW_OMNISCIENCE

    return DIRECTION_TO_CHAR.at(m_mouse->getCurrentDiscretizedRotation());
}

double MouseInterface::currentXPosMeters() {

    ENSURE_ALLOW_OMNISCIENCE

    return m_mouse->getCurrentTranslation().getX().getMeters();
}

double MouseInterface::currentYPosMeters() {

    ENSURE_ALLOW_OMNISCIENCE

    return m_mouse->getCurrentTranslation().getY().getMeters();
}

double MouseInterface::currentRotationDegrees() {

    ENSURE_ALLOW_OMNISCIENCE

    return m_mouse->getCurrentRotation().getDegreesZeroTo360();
}

void MouseInterface::ensureDiscreteInterface(const std::string& callingFunction) const {
    if (m_options.interfaceType != InterfaceType::DISCRETE) {
        L()->error(
            "You must declare the interface type to be \"%v\" to use MouseInterface::%v().",
            INTERFACE_TYPE_TO_STRING.at(InterfaceType::DISCRETE), callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureContinuousInterface(const std::string& callingFunction) const {
    if (m_options.interfaceType != InterfaceType::CONTINUOUS) {
        L()->error(
            "You must declare the interface type to be \"%v\" to use MouseInterface::%v().",
            INTERFACE_TYPE_TO_STRING.at(InterfaceType::CONTINUOUS), callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureAllowOmniscience(const std::string& callingFunction) const {
    if (!m_options.allowOmniscience) {
        L()->error(
            "You must return true from \"allowOmniscience()\" in order to use MouseInterface::%v().",
            callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureNotTileEdgeMovements(const std::string& callingFunction) const {
    if (m_options.useTileEdgeMovements) {
        L()->error(
            "You must return false from \"useTileEdgeMovements()\" in order to use MouseInterface::%v().",
            callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureUseTileEdgeMovements(const std::string& callingFunction) const {
    if (!m_options.useTileEdgeMovements) {
        L()->error(
            "You must return true from \"useTileEdgeMovements()\" in order to use MouseInterface::%v().",
            callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::declareWallImpl(std::pair<std::pair<int, int>, Direction> wall, bool wallExists, bool declareBothWallHalves) {
    m_mazeGraphic->declareWall(wall.first.first, wall.first.second, wall.second, wallExists); 
    if (declareBothWallHalves && hasOpposingWall(wall)) {
        declareWallImpl(getOpposingWall(wall), wallExists, false);
    }
}

void MouseInterface::undeclareWallImpl(std::pair<std::pair<int, int>, Direction> wall, bool declareBothWallHalves) {
    m_mazeGraphic->undeclareWall(wall.first.first, wall.first.second, wall.second); 
    if (declareBothWallHalves && hasOpposingWall(wall)) {
        undeclareWallImpl(getOpposingWall(wall), false);
    }
}

bool MouseInterface::wallFrontImpl(bool declareWallOnRead, bool declareBothWallHalves) {
    return isWall(
        std::make_pair(
            m_mouse->getCurrentDiscretizedTranslation(),
            m_mouse->getCurrentDiscretizedRotation()
        ),
        declareWallOnRead,
        declareBothWallHalves
    );
}

bool MouseInterface::wallLeftImpl(bool declareWallOnRead, bool declareBothWallHalves) {
    return isWall(
        std::make_pair(
            m_mouse->getCurrentDiscretizedTranslation(),
            DIRECTION_ROTATE_LEFT.at(m_mouse->getCurrentDiscretizedRotation())
        ),
        declareWallOnRead,
        declareBothWallHalves
    );
}

bool MouseInterface::wallRightImpl(bool declareWallOnRead, bool declareBothWallHalves) {
    return isWall(
        std::make_pair(
            m_mouse->getCurrentDiscretizedTranslation(),
            DIRECTION_ROTATE_RIGHT.at(m_mouse->getCurrentDiscretizedRotation())
        ),
        declareWallOnRead,
        declareBothWallHalves
    );
}

bool MouseInterface::isWall(std::pair<std::pair<int, int>, Direction> wall, bool declareWallOnRead, bool declareBothWallHalves) {

    int x = wall.first.first;
    int y = wall.first.second;
    Direction direction = wall.second;

    ASSERT_TR(m_maze->withinMaze(x, y));

    bool wallExists = m_maze->getTile(x, y)->isWall(direction);

    if (declareWallOnRead) {
        declareWallImpl(wall, wallExists, declareBothWallHalves);
    }

    return wallExists;
}

bool MouseInterface::hasOpposingWall(std::pair<std::pair<int, int>, Direction> wall) const {
    int x = wall.first.first;
    int y = wall.first.second;
    Direction direction = wall.second;
    switch (direction) {
        case Direction::NORTH:
            return y < m_maze->getHeight() - 1;
        case Direction::EAST:
            return x < m_maze->getWidth() - 1;
        case Direction::SOUTH:
            return 0 < y;
        case Direction::WEST:
            return 0 < x;
    }
}

std::pair<std::pair<int, int>, Direction> MouseInterface::getOpposingWall(
        std::pair<std::pair<int, int>, Direction> wall) const {
    ASSERT_TR(hasOpposingWall(wall));
    int x = wall.first.first;
    int y = wall.first.second;
    Direction direction = wall.second;
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

void MouseInterface::turnTo(const Cartesian& destinationTranslation, const Radians& destinationRotation) {

    // Determine the inital rotation delta in [-180, 180)
    Radians initialRotationDelta = getRotationDelta(m_mouse->getCurrentRotation(), destinationRotation);

    // Set the speed based on the initial rotation delta
    if (0 < initialRotationDelta.getDegreesNotBounded()) {
        m_mouse->setWheelSpeedsForTurnLeft(m_options.wheelSpeedFraction / 2.0);
    }
    else {
        m_mouse->setWheelSpeedsForTurnRight(m_options.wheelSpeedFraction / 2.0);
    }
    
    // While the deltas have the same sign, sleep for a short amount of time
    while (0 <
            initialRotationDelta.getRadiansNotBounded() *
            getRotationDelta(
                m_mouse->getCurrentRotation(),
                destinationRotation
            ).getRadiansNotBounded()) {
        sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
    }

    // Stop the wheels and teleport to the exact destination
    m_mouse->stopAllWheels();
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::moveForwardTo(const Cartesian& destinationTranslation, const Radians& destinationRotation) {

    // TODO: MACK - this doesn't work well if we're already past the point

    // This function assumes that we're already facing the correct direction,
    // and that we simply need to move forward to reach the destination.

    // Determine initial angle between the two points
    Degrees initialAngle = (destinationTranslation - m_mouse->getCurrentTranslation()).getTheta();
    Degrees currentAngle = initialAngle;

    // Start the mouse moving forward
    m_mouse->setWheelSpeedsForMoveForward(m_options.wheelSpeedFraction);

    // While the angle delta is not ~180 degrees, sleep for a short amout of time
    while (std::abs((currentAngle - initialAngle).getDegreesZeroTo360()) <  90
        || std::abs((currentAngle - initialAngle).getDegreesZeroTo360()) > 270) {
        sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
        currentAngle = (destinationTranslation - m_mouse->getCurrentTranslation()).getTheta();
    }

    // Stop the wheels and teleport to the exact destination
    m_mouse->stopAllWheels();
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::arcTo(const Cartesian& destinationTranslation, const Radians& destinationRotation, const Meters& radius) {

    // TODO: MACK - fix/test this

    // Determine the inital rotation delta in [-180, 180)
    Radians initialRotationDelta = getRotationDelta(m_mouse->getCurrentRotation(), destinationRotation);

    // Set the speed based on the initial rotation delta
    if (0 < initialRotationDelta.getDegreesNotBounded()) {
        m_mouse->setWheelSpeedsForCurveLeft(m_options.wheelSpeedFraction, radius);
    }
    else {
        m_mouse->setWheelSpeedsForCurveRight(m_options.wheelSpeedFraction, radius);
    }
    
    // While the deltas have the same sign, sleep for a short amount of time
    while (0 <
            initialRotationDelta.getRadiansNotBounded() *
            getRotationDelta(
                m_mouse->getCurrentRotation(),
                destinationRotation
            ).getRadiansNotBounded()) {
        sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
    }

    // Stop the wheels and teleport to the exact destination
    m_mouse->stopAllWheels();
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

Radians MouseInterface::getRotationDelta(const Radians& from, const Radians& to) const {
    static const Degrees lowerBound = Degrees(-180);
    static const Degrees upperBound = Degrees(180);
    static const Degrees fullCircle = Degrees(360);
    Radians delta = Radians(to.getRadiansZeroTo2pi() - from.getRadiansZeroTo2pi());
    if (delta.getRadiansNotBounded() < lowerBound.getRadiansNotBounded()) {
        delta += fullCircle;
    }
    if (upperBound.getRadiansNotBounded() <= delta.getRadiansNotBounded()) {
        delta -= fullCircle;
    }
    ASSERT_LE(lowerBound.getRadiansNotBounded(), delta.getRadiansNotBounded());
    ASSERT_LT(delta.getRadiansNotBounded(), upperBound.getRadiansNotBounded());
    return delta;
}

Cartesian MouseInterface::getDestinationTranslationForMoveForward() const {

    // TODO: MACK - clean this up..., use polar values to help...

    // Get the length of a single tile
    Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());

    // We modify these values in the switch statement
    Cartesian destinationTranslation = m_mouse->getCurrentTranslation();
    Degrees destinationRotation = m_mouse->getCurrentRotation();

    // TODO: MACK - make special methods???
    Meters halfWallWidth = Meters(P()->wallWidth() / 2.0);
    if (m_options.useTileEdgeMovements) {
        destinationTranslation = Cartesian(
            tileLength * m_mouse->getCurrentDiscretizedTranslation().first,
            tileLength * m_mouse->getCurrentDiscretizedTranslation().second
        );
    }

    switch (m_mouse->getCurrentDiscretizedRotation()) {
        case Direction::NORTH: {
            if (m_options.useTileEdgeMovements) {
                destinationTranslation += Cartesian(tileLength / 2.0, tileLength + halfWallWidth);
            }
            else {
                destinationTranslation += Cartesian(Meters(0), tileLength);
            }
            break;
        }
        case Direction::EAST: {
            if (m_options.useTileEdgeMovements) {
                destinationTranslation += Cartesian(tileLength + halfWallWidth, tileLength / 2.0);
            }
            else {
                destinationTranslation += Cartesian(tileLength, Meters(0));
            }
            break;
        }
        case Direction::SOUTH: {
            if (m_options.useTileEdgeMovements) {
                destinationTranslation += Cartesian(tileLength / 2.0, halfWallWidth * -1);
            }
            else {
                destinationTranslation += Cartesian(Meters(0), tileLength * -1);
            }
            break;
        }
        case Direction::WEST: {
            if (m_options.useTileEdgeMovements) {
                destinationTranslation += Cartesian(halfWallWidth * -1, tileLength / 2.0);
            }
            else {
                destinationTranslation += Cartesian(tileLength * -1, Meters(0));
            }
            break;
        }
    }

    return destinationTranslation;
}

} // namespace sim

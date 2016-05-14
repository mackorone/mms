#include "MouseInterface.h"

#include "units/Meters.h"
#include "units/MetersPerSecond.h"
#include "units/Milliseconds.h"
#include "units/Polar.h"
#include "units/RevolutionsPerMinute.h"
#include "units/Seconds.h"

#include "../mouse/IMouseAlgorithm.h"
#include "Assert.h"
#include "Color.h"
#include "ContainerUtilities.h"
#include "CPMath.h"
#include "Logging.h"
#include "Param.h"
#include "State.h"
#include "SimUtilities.h"
#include "Time.h"

namespace sim {

MouseInterface::MouseInterface(
        const Maze* maze,
        Mouse* mouse,
        MazeGraphic* mazeGraphic,
        IMouseAlgorithm* mouseAlgorithm,
        std::set<char> allowableTileTextCharacters,
        StaticMouseAlgorithmOptions options) :
        m_maze(maze),
        m_mouse(mouse),
        m_mazeGraphic(mazeGraphic),
        m_mouseAlgorithm(mouseAlgorithm),
        m_allowableTileTextCharacters(allowableTileTextCharacters),
        m_options(options),
        m_inOrigin(true) {
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
    return T()->elapsedSimTime().getMilliseconds();
}

void MouseInterface::delay(int milliseconds) {
    Seconds start = T()->elapsedSimTime();
    while (T()->elapsedSimTime() < start + Milliseconds(milliseconds)) {
        sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
    }
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

    setTileColorImpl(x, y, color);
}

void MouseInterface::clearTileColor(int x, int y) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and"
            " thus you cannot clear its color.", x, y);
        return;
    }

    clearTileColorImpl(x, y);
}

void MouseInterface::clearAllTileColor() {
    for (std::pair<int, int> position : m_tilesWithColor) {
        clearTileColorImpl(position.first, position.second);
    }
}

void MouseInterface::setTileText(int x, int y, const std::string& text) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and thus"
            " you cannot set its text to \"%v\".", x, y, text);
        return;
    }

    setTileTextImpl(x, y, text);
}

void MouseInterface::clearTileText(int x, int y) {

    if (!m_maze->withinMaze(x, y)) {
        L()->warn(
            "There is no tile at position (%v, %v), and"
            " thus you cannot clear its text.", x, y);
        return;
    }

    clearTileTextImpl(x, y);
}

void MouseInterface::clearAllTileText() {
    for (std::pair<int, int> position : m_tilesWithText) {
        clearTileTextImpl(position.first, position.second);
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
        m_mouseAlgorithm->declareBothWallHalves()
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
        m_mouseAlgorithm->declareBothWallHalves()
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

    if (m_mouseAlgorithm->setTileTextWhenDistanceDeclared()) {
        setTileTextImpl(x, y, (0 <= distance ? std::to_string(distance) : "inf"));
    }
    if (m_mouseAlgorithm->setTileBaseColorWhenDistanceDeclaredCorrectly()) {
        int actualDistance = m_maze->getTile(x, y)->getDistance();
        // A negative distance is interpreted to mean infinity
        if (distance == actualDistance || (distance < 0 && actualDistance < 0)) {
            setTileColorImpl(x, y,
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

    if (m_mouseAlgorithm->setTileTextWhenDistanceDeclared()) {
        clearTileTextImpl(x, y);
    }
    if (m_mouseAlgorithm->setTileBaseColorWhenDistanceDeclaredCorrectly()) {
        setTileColorImpl(x, y, COLOR_TO_CHAR.at(STRING_TO_COLOR.at(P()->tileBaseColor())));
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

    return m_mouse->readSensor(name);
}

double MouseInterface::readGyro() {

    ENSURE_CONTINUOUS_INTERFACE

    return m_mouse->readGyro().getDegreesPerSecond();
}

bool MouseInterface::wallFront() {

    ENSURE_DISCRETE_INTERFACE

    return wallFrontImpl(
        m_mouseAlgorithm->declareWallOnRead(),
        m_mouseAlgorithm->declareBothWallHalves()
    );
}

bool MouseInterface::wallRight() {

    ENSURE_DISCRETE_INTERFACE

    return wallRightImpl(
        m_mouseAlgorithm->declareWallOnRead(),
        m_mouseAlgorithm->declareBothWallHalves()
    );
}

bool MouseInterface::wallLeft() {

    ENSURE_DISCRETE_INTERFACE

    return wallLeftImpl(
        m_mouseAlgorithm->declareWallOnRead(),
        m_mouseAlgorithm->declareBothWallHalves()
    );
}

void MouseInterface::moveForward() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    moveForwardImpl();
}

void MouseInterface::moveForward(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    for (int i = 0; i < count; i += 1) {
        moveForwardImpl();
    }
}

void MouseInterface::turnLeft() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    turnLeftImpl();
}

void MouseInterface::turnRight() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    turnRightImpl();
}

void MouseInterface::turnAroundLeft() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    turnAroundLeftImpl();
}

void MouseInterface::turnAroundRight() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_NOT_TILE_EDGE_MOVEMENTS

    turnAroundRightImpl();
}

void MouseInterface::originMoveForwardToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_INSIDE_ORIGIN

    moveForwardImpl(true);
    m_inOrigin = false;
}

void MouseInterface::originTurnLeftInPlace() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_INSIDE_ORIGIN

    turnLeftImpl();
}

void MouseInterface::originTurnRightInPlace() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_INSIDE_ORIGIN

    turnRightImpl();
}

void MouseInterface::moveForwardToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    moveForwardImpl();
}

void MouseInterface::moveForwardToEdge(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    for (int i = 0; i < count; i += 1) {
        moveForwardImpl();
    }
}

void MouseInterface::turnLeftToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    turnToEdgeImpl(true);
}

void MouseInterface::turnRightToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    turnToEdgeImpl(false);
}

void MouseInterface::turnAroundLeftToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    turnAroundToEdgeImpl(true);
}

void MouseInterface::turnAroundRightToEdge() {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    turnAroundToEdgeImpl(false);
}

void MouseInterface::diagonalLeftLeft(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    doDiagonal(count, true, true);
}

void MouseInterface::diagonalLeftRight(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    doDiagonal(count, true, false);
}

void MouseInterface::diagonalRightLeft(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    doDiagonal(count, false, true);
}

void MouseInterface::diagonalRightRight(int count) {

    ENSURE_DISCRETE_INTERFACE
    ENSURE_USE_TILE_EDGE_MOVEMENTS
    ENSURE_OUTSIDE_ORIGIN

    doDiagonal(count, false, false);
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
    if (STRING_TO_INTERFACE_TYPE.at(m_options.interfaceType) != InterfaceType::DISCRETE) {
        L()->error(
            "You must declare the interface type to be \"%v\" to use MouseInterface::%v().",
            INTERFACE_TYPE_TO_STRING.at(InterfaceType::DISCRETE), callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureContinuousInterface(const std::string& callingFunction) const {
    if (STRING_TO_INTERFACE_TYPE.at(m_options.interfaceType) != InterfaceType::CONTINUOUS) {
        L()->error(
            "You must declare the interface type to be \"%v\" to use MouseInterface::%v().",
            INTERFACE_TYPE_TO_STRING.at(InterfaceType::CONTINUOUS), callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureAllowOmniscience(const std::string& callingFunction) const {
    if (!m_mouseAlgorithm->allowOmniscience()) {
        L()->error(
            "You must return true from \"allowOmniscience()\" in order to use MouseInterface::%v().",
            callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureNotTileEdgeMovements(const std::string& callingFunction) const {
    if (m_mouseAlgorithm->useTileEdgeMovements()) {
        L()->error(
            "You must return false from \"useTileEdgeMovements()\" in order to use MouseInterface::%v().",
            callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureUseTileEdgeMovements(const std::string& callingFunction) const {
    if (!m_mouseAlgorithm->useTileEdgeMovements()) {
        L()->error(
            "You must return true from \"useTileEdgeMovements()\" in order to use MouseInterface::%v().",
            callingFunction);
        SimUtilities::quit();
    }
}

void MouseInterface::ensureInsideOrigin(const std::string& callingFunction) const {
    if (!m_inOrigin) {
        L()->error(
            "You should only call MouseInterface::%v() if you're in the"
            " origin, i.e., you haven't moved forward at all yet.",
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

void MouseInterface::setTileColorImpl(int x, int y, char color) {
    m_mazeGraphic->setTileColor(x, y, CHAR_TO_COLOR.at(color));
    m_tilesWithColor.insert(std::make_pair(x, y));
}

void MouseInterface::clearTileColorImpl(int x, int y) {
    m_mazeGraphic->setTileColor(x, y, STRING_TO_COLOR.at(P()->tileBaseColor()));
    m_tilesWithColor.erase(std::make_pair(x, y));
}

void MouseInterface::setTileTextImpl(int x, int y, const std::string& text) {
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

void MouseInterface::clearTileTextImpl(int x, int y) {
    m_mazeGraphic->setTileText(x, y, {});
    m_tilesWithText.erase(std::make_pair(x, y));
}

void MouseInterface::declareWallImpl(
        std::pair<std::pair<int, int>, Direction> wall, bool wallExists, bool declareBothWallHalves) {
    m_mazeGraphic->declareWall(wall.first.first, wall.first.second, wall.second, wallExists); 
    if (declareBothWallHalves && hasOpposingWall(wall)) {
        declareWallImpl(getOpposingWall(wall), wallExists, false);
    }
}

void MouseInterface::undeclareWallImpl(
        std::pair<std::pair<int, int>, Direction> wall, bool declareBothWallHalves) {
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

void MouseInterface::moveForwardImpl(bool originMoveForwardToEdge) {

    static Meters halfWallLengthPlusWallWidth = Meters(P()->wallLength() / 2.0 + P()->wallWidth());
    static Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    static Meters wallWidth = Meters(P()->wallWidth());

    // Whether or not this movement will cause a crash
    bool crash = wallFrontImpl(false, false);

    // Get the location of the crash, if it will happen
    std::pair<Cartesian, Degrees> crashLocation = getCrashLocation(
        m_mouse->getCurrentDiscretizedTranslation(),
        m_mouse->getCurrentDiscretizedRotation()
    );

    // Get the destination translation of the mouse
    Cartesian destinationTranslation = m_mouse->getCurrentTranslation() +
        Polar(
            (originMoveForwardToEdge ? halfWallLengthPlusWallWidth : tileLength),
            crashLocation.second
        );

    // Move forward to the crash location
    moveForwardTo(crashLocation.first, crashLocation.second);

    // If we didn't crash, finish the move forward
    if (!crash) {
        moveForwardTo(destinationTranslation, crashLocation.second);
    }

    // Otherwise, set the crashed state (if it hasn't already been set)
    else if (!S()->crashed()) {
        S()->setCrashed();
    }
}

void MouseInterface::turnLeftImpl() {
    turnTo(m_mouse->getCurrentTranslation(), m_mouse->getCurrentRotation() + Degrees(90));
}

void MouseInterface::turnRightImpl() {
    turnTo(m_mouse->getCurrentTranslation(), m_mouse->getCurrentRotation() - Degrees(90));
}

void MouseInterface::turnAroundLeftImpl() {
    for (int i = 0; i < 2; i += 1) {
        turnLeftImpl();
    }
}

void MouseInterface::turnAroundRightImpl() {
    for (int i = 0; i < 2; i += 1) {
        turnRightImpl();
    }
}

void MouseInterface::turnAroundToEdgeImpl(bool turnLeft) {

    // Move to the center of the tile
    Cartesian delta = Polar(Meters(P()->wallLength() / 2.0), m_mouse->getCurrentRotation());
    moveForwardTo(m_mouse->getCurrentTranslation() + delta, m_mouse->getCurrentRotation());

    // Turn around
    if (turnLeft) {
        turnAroundLeftImpl();
    }
    else {
        turnAroundRightImpl();
    }

    // Move forward, into the next tile
    delta = Polar(Meters(P()->wallLength() / 2.0 + P()->wallWidth()), m_mouse->getCurrentRotation());
    moveForwardTo(m_mouse->getCurrentTranslation() + delta, m_mouse->getCurrentRotation());
}

void MouseInterface::turnToEdgeImpl(bool turnLeft) {

    static Meters halfWallLength = Meters(P()->wallLength() / 2.0);
    static Meters wallWidth = Meters(P()->wallWidth());

    // Whether or not this movement will cause a crash
    bool crash = (
        ( turnLeft &&  wallLeftImpl(false, false)) ||
        (!turnLeft && wallRightImpl(false, false))
    );

    // Get the location of the crash, if it will happen
    std::pair<Cartesian, Degrees> crashLocation = getCrashLocation(
        m_mouse->getCurrentDiscretizedTranslation(),
        (
            turnLeft ?
            DIRECTION_ROTATE_LEFT.at(m_mouse->getCurrentDiscretizedRotation()) :
            DIRECTION_ROTATE_RIGHT.at(m_mouse->getCurrentDiscretizedRotation())
        )
    );

    // Perform the curve turn
    arcTo(crashLocation.first, crashLocation.second, halfWallLength, 1.0);

    // If we didn't crash, move forward into the new tile
    if (!crash) {
        moveForwardTo(
            crashLocation.first + Polar(wallWidth, crashLocation.second),
            crashLocation.second
        );
    }

    // Otherwise, set the crashed state (if it hasn't already been set)
    else if (!S()->crashed()) {
        S()->setCrashed();
    }
}

bool MouseInterface::isWall(std::pair<std::pair<int, int>, Direction> wall, bool declareWallOnRead, bool declareBothWallHalves) {

    int x = wall.first.first;
    int y = wall.first.second;
    Direction direction = wall.second;

    SIM_ASSERT_TR(m_maze->withinMaze(x, y));

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
    SIM_ASSERT_TR(hasOpposingWall(wall));
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

void MouseInterface::moveForwardTo(const Cartesian& destinationTranslation, const Radians& destinationRotation) {

    // This function assumes that we're already facing the correct direction,
    // and that we simply need to move forward to reach the destination.

    // Determine delta between the two points
    Polar delta = destinationTranslation - m_mouse->getCurrentTranslation();
    Degrees initialAngle = delta.getTheta();
    Meters previousDistance = delta.getRho();

    // Start the mouse moving forward
    m_mouse->setWheelSpeedsForMoveForward(m_options.wheelSpeedFraction);

    // Move forward until we've reached the destination
    do {
        // Assert that we're actually moving closer to the destination
        SIM_ASSERT_LE(delta.getRho().getMeters(), previousDistance.getMeters());
        previousDistance = delta.getRho();
        // Update the translation delta
        sim::SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
        delta = destinationTranslation - m_mouse->getCurrentTranslation();
    }
    // While the angle delta is not ~180 degrees, sleep for a short amout of time
    while (std::abs((delta.getTheta() - initialAngle).getDegreesZeroTo360()) <  90
        || std::abs((delta.getTheta() - initialAngle).getDegreesZeroTo360()) > 270);

    // Stop the wheels and teleport to the exact destination
    m_mouse->stopAllWheels();
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::arcTo(const Cartesian& destinationTranslation, const Radians& destinationRotation,
        const Meters& radius, double extraWheelSpeedFraction) {

    // Determine the inital rotation delta in [-180, 180)
    Radians initialRotationDelta = getRotationDelta(m_mouse->getCurrentRotation(), destinationRotation);

    // Set the speed based on the initial rotation delta
    if (0 < initialRotationDelta.getDegreesNotBounded()) {
        m_mouse->setWheelSpeedsForCurveLeft(
            m_options.wheelSpeedFraction * extraWheelSpeedFraction, radius);
    }
    else {
        m_mouse->setWheelSpeedsForCurveRight(
            m_options.wheelSpeedFraction * extraWheelSpeedFraction, radius);
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

void MouseInterface::turnTo(const Cartesian& destinationTranslation, const Radians& destinationRotation) {
    // When we're turning in place, we set the wheels to half speed
    arcTo(destinationTranslation, destinationRotation, Meters(0), 0.5);
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
    SIM_ASSERT_LE(lowerBound.getRadiansNotBounded(), delta.getRadiansNotBounded());
    SIM_ASSERT_LT(delta.getRadiansNotBounded(), upperBound.getRadiansNotBounded());
    return delta;
}

Cartesian MouseInterface::getCenterOfTile(int x, int y) const {
    SIM_ASSERT_TR(m_maze->withinMaze(x, y));
    static Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    Cartesian centerOfTile = Cartesian(
        tileLength * (static_cast<double>(x) + 0.5),
        tileLength * (static_cast<double>(y) + 0.5)
    );
    return centerOfTile;
}

std::pair<Cartesian, Degrees> MouseInterface::getCrashLocation(
        std::pair<int, int> currentTile, Direction destinationDirection) {

    static Meters halfWallLength = Meters(P()->wallLength() / 2.0);

    // The crash locations for each destinationDirection, (N)orth, (E)ast,
    // (S)outh, and (W)est, are as show below. Basically, they're on the edge
    // of the tile's inner polygon. This "crash location" is used as an
    // intermediate location for (almost) all discrete movements that could
    // potentially cause the mouse to crash. That is, it's where the mouse
    // stops if it does crash, so as to give the user an indication of where
    // the mouse went wrong.
    //
    //                      +---+-------------+---+
    //                      |   |             |   |
    //                      +---+------N------+---+
    //                      |   |             |   |
    //                      |   |             |   |
    //                      |   W             E   |
    //                      |   |             |   |
    //                      |   |             |   |
    //                      +---+------S------+---+
    //                      |   |             |   |
    //                      +---+-------------+---+

    // The crash location is on the edge of the tile inner polygon
    Cartesian centerOfTile = getCenterOfTile(currentTile.first, currentTile.second);
    Degrees destinationRotation = DIRECTION_TO_ANGLE.at(destinationDirection);
    return std::make_pair(
        centerOfTile + Polar(halfWallLength, destinationRotation),
        destinationRotation
    );
}

void MouseInterface::doDiagonal(int count, bool startLeft, bool endLeft) {

    // Don't do/print anything if the mouse has already crashed
    if (S()->crashed()) {
        return;
    }

    // Whether or not the mouse will crash
    bool crash = false;

    if (startLeft == endLeft) {
        if (count % 2 != 1) {
            L()->warn(
                "Turning left or right at both the entrance and exit of a"
                " diagonal requires that you specify and odd number of diagonal"
                " segments to traverse. You tried turning %v twice, but"
                " specified a segment count of %v. Your mouse will crash at the"
                " end of the movement.",
                (startLeft ? "left" : "right"),
                count
            );
            crash = true;
        }
    }

    else {
        if (count % 2 != 0) {
            L()->warn(
                "Turning left at the entrance and right at the exit (or vice"
                " versa) of a diagonal requires that you specify and even"
                " number of diagonal segments to traverse. You tried %v at the"
                " entrance of the curve turn, and %v at the exit, but you"
                " specified a segment count of %v. Your mouse will crash at the"
                " end of the movement.",
                (startLeft ? "left" : "right"),
                (endLeft ? "left" : "right"),
                count
            );
            crash = true;
        }
    }

    // TODO: MACK - make sure that the path is actually clear

    static Meters halfTileWidth = Meters(P()->wallLength() + P()->wallWidth()) / 2.0;
    static Meters halfTileDiagonal = Meters(std::sqrt(2 * (halfTileWidth * halfTileWidth).getMetersSquared()));

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

    if (crash && !S()->crashed()) {
        S()->setCrashed();
    }
}

} // namespace sim

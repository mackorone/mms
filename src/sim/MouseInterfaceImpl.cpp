#include "MouseInterfaceImpl.h"

#include "units/Meters.h"
#include "units/Milliseconds.h"
#include "units/Polar.h"
#include "units/RevolutionsPerMinute.h"

#include "Assert.h"
#include "Color.h"
#include "CPMath.h"
#include "Logging.h"
#include "Param.h"
#include "State.h"
#include "SimUtilities.h"

namespace sim {

MouseInterfaceImpl::MouseInterfaceImpl(
        const Maze* maze,
        Mouse* mouse,
        MazeGraphic* mazeGraphic) :
        m_maze(maze),
        m_mouse(mouse),
        m_mazeGraphic(mazeGraphic) {
}

void MouseInterfaceImpl::debug(const std::string& str) {
    Logging::getMouseLogger()->debug(str);
}

void MouseInterfaceImpl::info(const std::string& str) {
    Logging::getMouseLogger()->info(str);
}

void MouseInterfaceImpl::warn(const std::string& str) {
    Logging::getMouseLogger()->warn(str);
}

void MouseInterfaceImpl::error(const std::string& str) {
    Logging::getMouseLogger()->error(str);
}

double MouseInterfaceImpl::getRandom() {
    return SimUtilities::getRandom();
}

int MouseInterfaceImpl::millis() {
    return m_mouse->getElapsedSimTime().getMilliseconds();
}

void MouseInterfaceImpl::delay(int milliseconds) {
    sim::SimUtilities::sleep(Milliseconds(milliseconds / S()->simSpeed()));
}

void MouseInterfaceImpl::quit() {
    sim::SimUtilities::quit();
}

void MouseInterfaceImpl::setTileColor(int x, int y, char color) {
    m_mazeGraphic->setTileColor(x, y, CHAR_TO_COLOR.at(color));
    m_tilesWithColor.insert(std::make_pair(x, y));
}

void MouseInterfaceImpl::clearTileColor(int x, int y) {
    m_mazeGraphic->setTileColor(x, y, STRING_TO_COLOR.at(P()->tileBaseColor()));
    m_tilesWithColor.erase(std::make_pair(x, y));
}

void MouseInterfaceImpl::clearAllTileColor() {
    for (std::pair<int, int> position : m_tilesWithColor) {
        clearTileColor(position.first, position.second);
    }
}

void MouseInterfaceImpl::setTileText(int x, int y, const std::string& text,
        int tileTextNumberOfRows, int tileTextNumberOfCols) {
    std::vector<std::string> rowsOfText;
    int row = 0;
    int index = 0;
    while (row < tileTextNumberOfRows && index < text.size()) {
        std::string rowOfText;
        while (index < (row + 1) * tileTextNumberOfCols && index < text.size()) {
            rowOfText += text.at(index);
            index += 1;
        }
        rowsOfText.push_back(rowOfText); 
        row += 1;
    }
    m_mazeGraphic->setTileText(x, y, rowsOfText);
    m_tilesWithText.insert(std::make_pair(x, y));
}

void MouseInterfaceImpl::clearTileText(int x, int y) {
    m_mazeGraphic->setTileText(x, y, {});
    m_tilesWithText.erase(std::make_pair(x, y));
}

void MouseInterfaceImpl::clearAllTileText() {
    for (std::pair<int, int> position : m_tilesWithText) {
        clearTileText(position.first, position.second);
    }
}

void MouseInterfaceImpl::declareWall(
        std::pair<std::pair<int, int>, Direction> wall,
        bool wallExists, bool declareBothWallHalves) {
    m_mazeGraphic->declareWall(wall.first.first, wall.first.second, wall.second, wallExists); 
    if (declareBothWallHalves && hasOpposingWall(wall)) {
        declareWall(getOpposingWall(wall), wallExists, false);
    }
}

void MouseInterfaceImpl::undeclareWall(
        std::pair<std::pair<int, int>, Direction> wall, bool declareBothWallHalves) {
    m_mazeGraphic->undeclareWall(wall.first.first, wall.first.second, wall.second); 
    if (declareBothWallHalves && hasOpposingWall(wall)) {
        undeclareWall(getOpposingWall(wall), false);
    }
}

void MouseInterfaceImpl::setTileFogginess(int x, int y, bool foggy) {
    m_mazeGraphic->setTileFogginess(x, y, foggy);
}

void MouseInterfaceImpl::declareTileDistance(
        int x, int y, int distance,
        bool setTileTextWhenDistanceDeclared,
        bool setTileBaseColorWhenDistanceDeclaredCorrectly,
        int tileTextNumberOfRows, int tileTextNumberOfCols) {
    if (setTileTextWhenDistanceDeclared) {
        setTileText(x, y, (0 <= distance ? std::to_string(distance) : "inf"),
            tileTextNumberOfRows, tileTextNumberOfCols);
    }
    if (setTileBaseColorWhenDistanceDeclaredCorrectly) {
        if (distance == m_maze->getTile(x, y)->getDistance()) {
            setTileColor(x, y,
                COLOR_TO_CHAR.at(STRING_TO_COLOR.at(P()->distanceCorrectTileBaseColor())));
        }
    }
}

void MouseInterfaceImpl::undeclareTileDistance(int x, int y,
        bool setTileTextWhenDistanceDeclared,
        bool setTileBaseColorWhenDistanceDeclaredCorrectly) {
    if (setTileTextWhenDistanceDeclared) {
        clearTileText(x, y);
    }
    if (setTileBaseColorWhenDistanceDeclaredCorrectly) {
        setTileColor(x, y, COLOR_TO_CHAR.at(STRING_TO_COLOR.at(P()->tileBaseColor())));
    }
}

void MouseInterfaceImpl::resetPosition() {
    m_mouse->teleport(m_mouse->getInitialTranslation(), m_mouse->getInitialRotation());
}

bool MouseInterfaceImpl::inputButtonPressed(int inputButton) {
    return S()->inputButtonWasPressed(inputButton);
}

void MouseInterfaceImpl::acknowledgeInputButtonPressed(int inputButton) {
    S()->setInputButtonWasPressed(inputButton, false);
}

double MouseInterfaceImpl::getWheelMaxSpeed(const std::string& name) {
    return m_mouse->getWheelMaxSpeed(name).getRevolutionsPerMinute();
}

void MouseInterfaceImpl::setWheelSpeed(const std::string& name, double rpm) {
    m_mouse->setWheelSpeeds({{name, RevolutionsPerMinute(rpm)}});
}

double MouseInterfaceImpl::getWheelEncoderTicksPerRevolution(const std::string& name) {
    return m_mouse->getWheelEncoderTicksPerRevolution(name);
}

int MouseInterfaceImpl::readWheelEncoder(const std::string& name) {
    switch (m_mouse->getWheelEncoderType(name)) {
        case EncoderType::ABSOLUTE:
            return m_mouse->readWheelAbsoluteEncoder(name);
        case EncoderType::RELATIVE:
            return m_mouse->readWheelRelativeEncoder(name);
    }
}

void MouseInterfaceImpl::resetWheelEncoder(const std::string& name) {
    m_mouse->resetWheelRelativeEncoder(name);
}

double MouseInterfaceImpl::readSensor(std::string name) {
    // TODO: MACK - test this
    //sim::SimUtilities::sleep(m_mouse->getSensorReadDuration(name));
    return m_mouse->readSensor(name);
}

double MouseInterfaceImpl::readGyro() {
    return m_mouse->readGyro().getDegreesPerSecond();
}

bool MouseInterfaceImpl::wallFront(bool declareWallOnRead, bool declareBothWallHalves) {
    return isWall(
        std::make_pair(
            m_mouse->getCurrentDiscretizedTranslation(),
            m_mouse->getCurrentDiscretizedRotation()
        ),
        declareWallOnRead,
        declareBothWallHalves
    );
}

bool MouseInterfaceImpl::wallLeft(bool declareWallOnRead, bool declareBothWallHalves) {
    return isWall(
        std::make_pair(
            m_mouse->getCurrentDiscretizedTranslation(),
            DIRECTION_ROTATE_LEFT.at(m_mouse->getCurrentDiscretizedRotation())
        ),
        declareWallOnRead,
        declareBothWallHalves
    );
}

bool MouseInterfaceImpl::wallRight(bool declareWallOnRead, bool declareBothWallHalves) {
    return isWall(
        std::make_pair(
            m_mouse->getCurrentDiscretizedTranslation(),
            DIRECTION_ROTATE_RIGHT.at(m_mouse->getCurrentDiscretizedRotation())
        ),
        declareWallOnRead,
        declareBothWallHalves
    );
}

void MouseInterfaceImpl::moveForward(double wheelSpeedFraction) {
    // TODO: MACK - still move forward
    if (wallFront(false, false)) {
        if (!S()->crashed()) {
            S()->setCrashed();
        }
        return;
    }
    static Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    Cartesian delta = Polar(tileLength, m_mouse->getCurrentRotation());
    moveForwardTo(m_mouse->getCurrentTranslation() + delta, m_mouse->getCurrentRotation(), wheelSpeedFraction);
}

void MouseInterfaceImpl::moveForward(int count, double wheelSpeedFraction) {
    for (int i = 0; i < count; i += 1) {
        moveForward(wheelSpeedFraction);
    }
}

void MouseInterfaceImpl::turnLeft(double wheelSpeedFraction) {
    turnTo(m_mouse->getCurrentTranslation(), m_mouse->getCurrentRotation() + Degrees(90), wheelSpeedFraction);
}

void MouseInterfaceImpl::turnRight(double wheelSpeedFraction) {
    turnTo(m_mouse->getCurrentTranslation(), m_mouse->getCurrentRotation() - Degrees(90), wheelSpeedFraction);
}

void MouseInterfaceImpl::turnAroundLeft(double wheelSpeedFraction) {
    for (int i = 0; i < 2; i += 1) {
        turnLeft(wheelSpeedFraction);
    }
}

void MouseInterfaceImpl::turnAroundRight(double wheelSpeedFraction) {
    for (int i = 0; i < 2; i += 1) {
        turnRight(wheelSpeedFraction);
    }
}

void MouseInterfaceImpl::originMoveForwardToEdge(double wheelSpeedFraction) {

    // TODO: MACK - still move forward
    if (wallFront(false, false)) {
        if (!S()->crashed()) {
            S()->setCrashed();
        }
        return;
    }

    std::pair<int, int> currentTile = m_mouse->getCurrentDiscretizedTranslation();
    Cartesian centerOfCurrentTile = getCenterOfTile(currentTile.first, currentTile.second);
    Cartesian delta = Polar(Meters(P()->wallLength() / 2.0 + P()->wallWidth()), m_mouse->getCurrentRotation());
    moveForwardTo(centerOfCurrentTile + delta, m_mouse->getCurrentRotation(), wheelSpeedFraction);
}

void MouseInterfaceImpl::originTurnLeftInPlace(double wheelSpeedFraction) {
    turnLeft(wheelSpeedFraction);
}

void MouseInterfaceImpl::originTurnRightInPlace(double wheelSpeedFraction) {
    turnRight(wheelSpeedFraction);
}

void MouseInterfaceImpl::moveForwardToEdge(double wheelSpeedFraction) {
    moveForward(wheelSpeedFraction);
}

void MouseInterfaceImpl::moveForwardToEdge(int count, double wheelSpeedFraction) {
    moveForward(count, wheelSpeedFraction);
}

void MouseInterfaceImpl::turnLeftToEdge(double wheelSpeedFraction) {
    curveTurn(true, wheelSpeedFraction);
}

void MouseInterfaceImpl::turnRightToEdge(double wheelSpeedFraction) {
    curveTurn(false, wheelSpeedFraction);
}

void MouseInterfaceImpl::turnAroundLeftToEdge(double wheelSpeedFraction) {
    turnAroundToEdge(true, wheelSpeedFraction);
}

void MouseInterfaceImpl::turnAroundRightToEdge(double wheelSpeedFraction) {
    turnAroundToEdge(false, wheelSpeedFraction);
}

void MouseInterfaceImpl::diagonalLeftLeft(int count, double wheelSpeedFraction) {
    doDiagonal(count, true, true, wheelSpeedFraction);
}

void MouseInterfaceImpl::diagonalLeftRight(int count, double wheelSpeedFraction) {
    doDiagonal(count, true, false, wheelSpeedFraction);
}

void MouseInterfaceImpl::diagonalRightLeft(int count, double wheelSpeedFraction) {
    doDiagonal(count, false, true, wheelSpeedFraction);
}

void MouseInterfaceImpl::diagonalRightRight(int count, double wheelSpeedFraction) {
    doDiagonal(count, false, false, wheelSpeedFraction);
}

int MouseInterfaceImpl::currentXTile() {
    return m_mouse->getCurrentDiscretizedTranslation().first;
}

int MouseInterfaceImpl::currentYTile() {
    return m_mouse->getCurrentDiscretizedTranslation().second;
}

char MouseInterfaceImpl::currentDirection() {
    return DIRECTION_TO_CHAR.at(m_mouse->getCurrentDiscretizedRotation());
}

double MouseInterfaceImpl::currentXPosMeters() {
    return m_mouse->getCurrentTranslation().getX().getMeters();
}

double MouseInterfaceImpl::currentYPosMeters() {
    return m_mouse->getCurrentTranslation().getY().getMeters();
}

double MouseInterfaceImpl::currentRotationDegrees() {
    return m_mouse->getCurrentRotation().getDegreesZeroTo360();
}

bool MouseInterfaceImpl::isWall(std::pair<std::pair<int, int>, Direction> wall, bool declareWallOnRead, bool declareBothWallHalves) {

    int x = wall.first.first;
    int y = wall.first.second;
    Direction direction = wall.second;

    ASSERT_TR(m_maze->withinMaze(x, y));

    bool wallExists = m_maze->getTile(x, y)->isWall(direction);

    if (declareWallOnRead) {
        declareWall(wall, wallExists, declareBothWallHalves);
    }

    return wallExists;
}

bool MouseInterfaceImpl::hasOpposingWall(std::pair<std::pair<int, int>, Direction> wall) const {
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

std::pair<std::pair<int, int>, Direction> MouseInterfaceImpl::getOpposingWall(
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

void MouseInterfaceImpl::curveTurn(bool turnLeft, double wheelSpeedFraction) {

    // Check for a crash
    // TODO: MACK - still move forward
    if ((turnLeft && wallLeft(false, false)) || (!turnLeft && wallRight(false, false))) {
        if (!S()->crashed()) {
            S()->setCrashed();
        }
        return;
    }

    Meters halfWallLength = Meters(P()->wallLength() / 2.0);
    std::pair<int, int> currentTile = m_mouse->getCurrentDiscretizedTranslation();
    Cartesian centerOfCurrentTile = getCenterOfTile(currentTile.first, currentTile.second);

    Degrees destinationRotation = m_mouse->getCurrentRotation() + Degrees((turnLeft ? 90 : -90));
    Cartesian intermediateDestination = centerOfCurrentTile + Polar(halfWallLength, destinationRotation);
    Cartesian finalDestination = intermediateDestination + Polar(Meters(P()->wallWidth()), destinationRotation);

    // Perform the curve turn
    arcTo(intermediateDestination, destinationRotation, halfWallLength, wheelSpeedFraction);
    moveForwardTo(finalDestination, destinationRotation, wheelSpeedFraction);
}

void MouseInterfaceImpl::turnAroundToEdge(bool turnLeft, double wheelSpeedFraction) {

    // Move to the center of the tile
    Cartesian delta = Polar(Meters(P()->wallLength() / 2.0), m_mouse->getCurrentRotation());
    moveForwardTo(m_mouse->getCurrentTranslation() + delta, m_mouse->getCurrentRotation(), wheelSpeedFraction);

    // Turn around
    if (turnLeft) {
        turnAroundLeft(wheelSpeedFraction);
    }
    else {
        turnAroundRight(wheelSpeedFraction);
    }

    // Move forward, into the next tile
    delta = Polar(Meters(P()->wallLength() / 2.0 + P()->wallWidth()), m_mouse->getCurrentRotation());
    moveForwardTo(m_mouse->getCurrentTranslation() + delta, m_mouse->getCurrentRotation(), wheelSpeedFraction);
}

void MouseInterfaceImpl::doDiagonal(int count, bool startLeft, bool endLeft, double wheelSpeedFraction) {

    // TODO: MACK - Clean this up
    // TODO: MACK - special case for counts 1,2
    // TODO: MACK - limits on count?

    if (startLeft == endLeft) {
        if (count % 2 != 1) {
        }
        ASSERT_EQ(count % 2, 1);
    }
    else {
        if (count % 2 != 0) {
        }
        ASSERT_EQ(count % 2, 0);
    }

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
    
    turnTo(m_mouse->getCurrentTranslation(), delta.getTheta(), wheelSpeedFraction);
    moveForwardTo(destination, m_mouse->getCurrentRotation(), wheelSpeedFraction);
    turnTo(m_mouse->getCurrentTranslation(), endRotation, wheelSpeedFraction);
    moveForwardTo(destination + Polar(Meters(P()->wallWidth() / 2.0), m_mouse->getCurrentRotation()),
        m_mouse->getCurrentRotation(), wheelSpeedFraction);
}

void MouseInterfaceImpl::moveForwardTo(const Cartesian& destinationTranslation, const Radians& destinationRotation,
        double wheelSpeedFraction) {

    // This function assumes that we're already facing the correct direction,
    // and that we simply need to move forward to reach the destination.

    // Determine delta between the two points
    Polar delta = destinationTranslation - m_mouse->getCurrentTranslation();
    Degrees initialAngle = delta.getTheta();
    Meters previousDistance = delta.getRho();

    // Start the mouse moving forward
    m_mouse->setWheelSpeedsForMoveForward(wheelSpeedFraction);

    // Move forward until we've reached the destination
    do {
        // Assert that we're actually moving closer to the destination
        ASSERT_LE(delta.getRho().getMeters(), previousDistance.getMeters());
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

void MouseInterfaceImpl::arcTo(const Cartesian& destinationTranslation, const Radians& destinationRotation,
        const Meters& radius, double wheelSpeedFraction) {

    // Determine the inital rotation delta in [-180, 180)
    Radians initialRotationDelta = getRotationDelta(m_mouse->getCurrentRotation(), destinationRotation);

    // Set the speed based on the initial rotation delta
    if (0 < initialRotationDelta.getDegreesNotBounded()) {
        m_mouse->setWheelSpeedsForCurveLeft(wheelSpeedFraction, radius);
    }
    else {
        m_mouse->setWheelSpeedsForCurveRight(wheelSpeedFraction, radius);
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

void MouseInterfaceImpl::turnTo(const Cartesian& destinationTranslation, const Radians& destinationRotation,
        double wheelSpeedFraction) {
    // When we're turning in place, we set the wheels to half speed
    arcTo(destinationTranslation, destinationRotation, Meters(0), wheelSpeedFraction * 0.5);
}

Radians MouseInterfaceImpl::getRotationDelta(const Radians& from, const Radians& to) const {
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

Cartesian MouseInterfaceImpl::getCenterOfTile(int x, int y) const {
    ASSERT_TR(m_maze->withinMaze(x, y));
    static Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    Cartesian centerOfTile = Cartesian(
        tileLength * (static_cast<double>(x) + 0.5),
        tileLength * (static_cast<double>(y) + 0.5)
    );
    return centerOfTile;
}

} // namespace sim

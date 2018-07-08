#include "MouseInterface.h"

#include <QChar>
#include <QDebug>
#include <QPair>
#include <QtMath>

#include "units/AngularVelocity.h"
#include "units/Distance.h"
#include "units/Duration.h"

#include "Assert.h"
#include "Color.h"
#include "ColorManager.h"
#include "FontImage.h"
#include "Logging.h"
#include "Param.h"
#include "SimTime.h"
#include "SimUtilities.h"

namespace mms {

MouseInterface::MouseInterface(
        const Maze* maze,
        Mouse* mouse,
        MazeView* view) :
        m_maze(maze),
        m_mouse(mouse),
        m_view(view),
        m_interfaceType(InterfaceType::DISCRETE),
        m_interfaceTypeFinalized(false),
        m_inOrigin(true),
        m_wheelSpeedFraction(1.0) {
}

void MouseInterface::emitMouseAlgoStarted() {
    emit mouseAlgoStarted();
}

QString MouseInterface::dispatch(const QString& command) {

    // TODO: upforgrabs
    // These functions should have sanity checks, e.g., correct
    // types, not finalizing static options more than once, etc.

    static const QString ACK_STRING = "ACK";
    static const QString NO_ACK_STRING = "";
    static const QString ERROR_STRING = "!";

    QStringList tokens = command.split(" ", QString::SkipEmptyParts);
    QString function = tokens.at(0);

    if (function == "getMazeWidth") {
        return QString::number(m_maze->getWidth());
    }
    else if (function == "getMazeHeight") {
        return QString::number(m_maze->getHeight());
    }
    else if (function == "isWallFront") {
        return SimUtilities::boolToStr(isWallFront());
    }
    else if (function == "isWallRight") {
        return SimUtilities::boolToStr(isWallRight());
    }
    else if (function == "isWallLeft") {
        return SimUtilities::boolToStr(isWallLeft());
    }
    else if (function == "moveForward") {
        moveForward();
        return ACK_STRING;
    }
    else if (function == "turnRight") {
        turnRight();
        return ACK_STRING;
    }
    else if (function == "turnLeft") {
        turnLeft();
        return ACK_STRING;
    }
    else if (function == "reset") {
        reset();
        return ACK_STRING;
    }
    else if (function == "setTileColor") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        char color = SimUtilities::strToChar(tokens.at(3));
        setTileColor(x, y, color);
        return NO_ACK_STRING;
    }
    else if (function == "clearTileColor") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        clearTileColor(x, y);
        return NO_ACK_STRING;
    }
    else if (function == "clearAllTileColor") {
        clearAllTileColor();
        return NO_ACK_STRING;
    }
    else if (function == "setTileText") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        QString text = "";
        if (3 < tokens.size()) {
            text = tokens.at(3);
        }
        setTileText(x, y, text);
        return NO_ACK_STRING;
    }
    else if (function == "clearTileText") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        clearTileText(x, y);
        return NO_ACK_STRING;
    }
    else if (function == "clearAllTileText") {
        clearAllTileText();
        return NO_ACK_STRING;
    }
    else if (function == "declareWall") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        char direction = SimUtilities::strToChar(tokens.at(3));
        bool wallExists = SimUtilities::strToBool(tokens.at(4));
        declareWall(x, y, direction, wallExists);
        return NO_ACK_STRING;
    }
    else if (function == "undeclareWall") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        char direction = SimUtilities::strToChar(tokens.at(3));
        undeclareWall(x, y, direction);
        return NO_ACK_STRING;
    }
    else if (function == "wasInputButtonPressed") {
        int inputButton = SimUtilities::strToInt(tokens.at(1));
        return SimUtilities::boolToStr(
            wasInputButtonPressed(inputButton)
        );
    }
    else if (function == "acknowledgeInputButtonPressed") {
        int inputButton = SimUtilities::strToInt(tokens.at(1));
        acknowledgeInputButtonPressed(inputButton);
        return ACK_STRING;
    }

    // TODO: MACK - print something here
    return ERROR_STRING;
}

void MouseInterface::inputButtonWasPressed(int button) {
    m_inputButtonsPressed[button] = true;
}

bool MouseInterface::isWallFront() {
    return isWall({
        m_mouse->getCurrentDiscretizedTranslation(),
        m_mouse->getCurrentDiscretizedRotation()
    });
}

bool MouseInterface::isWallRight() {
    return isWall({
        m_mouse->getCurrentDiscretizedTranslation(),
        DIRECTION_ROTATE_RIGHT().value(m_mouse->getCurrentDiscretizedRotation())
    });
}

bool MouseInterface::isWallLeft() {
    return isWall({
        m_mouse->getCurrentDiscretizedTranslation(),
        DIRECTION_ROTATE_LEFT().value(m_mouse->getCurrentDiscretizedRotation())
    });
}

void MouseInterface::moveForward() {
    moveForwardImpl();
}

void MouseInterface::turnRight() {
    turnRightImpl();
}

void MouseInterface::turnLeft() {
    turnLeftImpl();
}

void MouseInterface::reset() {
    m_mouse->reset();
}

void MouseInterface::setTileColor(int x, int y, char color) {

    if (!m_maze->withinMaze(x, y)) {
        qWarning().noquote().nospace()
            << "There is no tile at position (" << x << ", " << y << ") and"
            << " thus you cannot set its color.";
        return;
    }

    if (!CHAR_TO_COLOR().contains(color)) {
        qWarning().noquote().nospace()
            << "You cannot set the color of tile (" << x << ", " << y << ") to"
            << "'" << color << "' since '" << color << "' is not mapped to a"
            << " color.";
        return;
    }

    setTileColorImpl(x, y, color);
}

void MouseInterface::clearTileColor(int x, int y) {

    if (!m_maze->withinMaze(x, y)) {
        qWarning().noquote().nospace()
            << "There is no tile at position (" << x << ", " << y << "), and"
            << " thus you cannot clear its color.";
        return;
    }

    clearTileColorImpl(x, y);
    m_tilesWithColor -= {x, y};
}

void MouseInterface::clearAllTileColor() {
    for (QPair<int, int> position : m_tilesWithColor) {
        clearTileColorImpl(position.first, position.second);
    }
    m_tilesWithColor.clear();
}

void MouseInterface::setTileText(int x, int y, const QString& text) {

    if (!m_maze->withinMaze(x, y)) {
        qWarning().noquote().nospace()
            << "There is no tile at position (" << x << ", " << y << "), and"
            << " thus you cannot set its text to \"" << text << "\".";
        return;
    }

    setTileTextImpl(x, y, text);
}

void MouseInterface::clearTileText(int x, int y) {

    if (!m_maze->withinMaze(x, y)) {
        qWarning().noquote().nospace()
            << "There is no tile at position (" << x << ", " << y << "), and"
            << " thus you cannot clear its text.";
        return;
    }

    clearTileTextImpl(x, y);
    m_tilesWithText -= {x, y};
}

void MouseInterface::clearAllTileText() {
    for (QPair<int, int> position : m_tilesWithText) {
        clearTileTextImpl(position.first, position.second);
    }
    m_tilesWithText.clear();
}

void MouseInterface::declareWall(int x, int y, char direction, bool wallExists) {

    if (!m_maze->withinMaze(x, y)) {
        qWarning().noquote().nospace()
            << "There is no tile at position (" << x << ", " << y << "), and"
            << " thus you cannot declare any of its walls.";
        return;
    }

    if (!CHAR_TO_DIRECTION().contains(direction)) {
        qWarning().noquote().nospace()
            << "The character '" << direction << "' is not mapped to a valid"
            << " direction.";
        return;
    }

    Direction d = CHAR_TO_DIRECTION().value(direction);
    m_view->getMazeGraphic()->declareWall(x, y, d, wallExists); 
    if (hasOpposingWall(x, y, d)) {
        auto opposingWall = getOpposingWall(x, y, d);
        m_view->getMazeGraphic()->declareWall(
            opposingWall.first.first,
            opposingWall.first.second,
            opposingWall.second,
            wallExists
        ); 
    }
}

void MouseInterface::undeclareWall(int x, int y, char direction) {

    if (!m_maze->withinMaze(x, y)) {
        qWarning().noquote().nospace()
            << "There is no tile at position (" << x << ", " << y << "), and"
            << " thus you cannot undeclare any of its walls.";
        return;
    }

    if (!CHAR_TO_DIRECTION().contains(direction)) {
        qWarning().noquote().nospace()
            << "The character '" << direction << "' is not mapped to a valid"
            << " direction.";
        return;
    }

    Direction d = CHAR_TO_DIRECTION().value(direction);
    m_view->getMazeGraphic()->undeclareWall(x, y, d);
    if (hasOpposingWall(x, y, d)) {
        auto opposingWall = getOpposingWall(x, y, d);
        m_view->getMazeGraphic()->undeclareWall(
            opposingWall.first.first,
            opposingWall.first.second,
            opposingWall.second
        ); 
    }
}

bool MouseInterface::wasInputButtonPressed(int inputButton) {

    if (inputButton < 0 || 9 < inputButton) {
        qWarning().noquote().nospace()
            << "There is no input button with the number " << inputButton << ","
            << " and thus you cannot check to see if it has been pressed.";
        return false;
    }

    return m_inputButtonsPressed.value(inputButton, false);
}

void MouseInterface::acknowledgeInputButtonPressed(int inputButton) {

    // TODO: upforgrabs
    // This shouldn't be hardcoded here; instead, it should be derived
    // from the Window (which actually contains the input buttons)
    if (inputButton < 0 || 9 < inputButton) {
        qWarning().noquote().nospace()
            << "There is no input button with the number " << inputButton << ","
            << " and thus you cannot acknowledge that it has been pressed.";
        return;
    }

    m_inputButtonsPressed[inputButton] = false;
    emit inputButtonWasAcknowledged(inputButton);
}

void MouseInterface::setTileColorImpl(int x, int y, char color) {
    m_view->getMazeGraphic()->setTileColor(x, y, CHAR_TO_COLOR().value(color));
    m_tilesWithColor.insert({x, y});
}

void MouseInterface::clearTileColorImpl(int x, int y) {
    m_view->getMazeGraphic()->setTileColor(
        x, y, ColorManager::get()->getTileBaseColor());
}

void MouseInterface::setTileTextImpl(int x, int y, const QString& text) {

    // Ensure that all characters are valid
    QString filtered;
    for (int i = 0; i < text.size(); i += 1) {
        QChar c = text.at(i);
        if (!FontImage::get()->positions().contains(c)) {
            qWarning().noquote().nospace()
                << "Unable to set the tile text for unprintable character \""
                << (c == '\n' ? "\\n" :
                   (c == '\t' ? "\\t" :
                   (c == '\r' ? "\\r" : QString(c))))
                << "\". Using the character \"?\" instead.";
            c = '?';
        }
        filtered += c;
    }

    m_view->getMazeGraphic()->setTileText(x, y, filtered);
    m_tilesWithText.insert({x, y});
}

void MouseInterface::clearTileTextImpl(int x, int y) {
    m_view->getMazeGraphic()->setTileText(x, y, {});
}

void MouseInterface::moveForwardImpl() {

    static Distance halfWallLengthPlusWallWidth =
        Distance::Meters(P()->wallLength() / 2.0 + P()->wallWidth());
    static Distance tileLength = Distance::Meters(P()->wallLength() + P()->wallWidth());
    static Distance wallWidth = Distance::Meters(P()->wallWidth());

    // Whether or not this movement will cause a crash
    bool crash = isWallFront();

    // Get the location of the crash, if it will happen
    QPair<Coordinate, Angle> crashLocation = getCrashLocation(
        m_mouse->getCurrentDiscretizedTranslation(),
        m_mouse->getCurrentDiscretizedRotation()
    );

    // Get the destination translation of the mouse
    Coordinate destinationTranslation =
        m_mouse->getCurrentTranslation() +
        Coordinate::Polar(tileLength, crashLocation.second);

    // Move forward to the crash location
    moveForwardTo(crashLocation.first, crashLocation.second);

    // If we didn't crash, finish the move forward
    if (!crash) {
        moveForwardTo(destinationTranslation, crashLocation.second);
    }

    // Otherwise, set the crashed state (if it hasn't already been set)
    else if (!m_mouse->didCrash()) {
        m_mouse->setCrashed();
    }
}

void MouseInterface::turnLeftImpl() {
    turnTo(m_mouse->getCurrentTranslation(), m_mouse->getCurrentRotation() + Angle::Degrees(90));
}

void MouseInterface::turnRightImpl() {
    turnTo(m_mouse->getCurrentTranslation(), m_mouse->getCurrentRotation() - Angle::Degrees(90));
}

bool MouseInterface::isWall(QPair<QPair<int, int>, Direction> wall) {
    int x = wall.first.first;
    int y = wall.first.second;
    Direction direction = wall.second;
    ASSERT_TR(m_maze->withinMaze(x, y));
    return m_maze->getTile(x, y)->isWall(direction);
}

bool MouseInterface::hasOpposingWall(int x, int y, Direction d) const {
    switch (d) {
        case Direction::NORTH:
            return y < m_maze->getHeight() - 1;
        case Direction::EAST:
            return x < m_maze->getWidth() - 1;
        case Direction::SOUTH:
            return 0 < y;
        case Direction::WEST:
            return 0 < x;
        default:
            ASSERT_NEVER_RUNS();
    }
}

QPair<QPair<int, int>, Direction> MouseInterface::getOpposingWall(
    int x,
    int y,
    Direction d
) const {
    ASSERT_TR(hasOpposingWall(x, y, d));
    switch (d) {
        case Direction::NORTH:
            return {{x, y + 1}, Direction::SOUTH};
        case Direction::EAST:
            return {{x + 1, y}, Direction::WEST};
        case Direction::SOUTH:
            return {{x, y - 1}, Direction::NORTH};
        case Direction::WEST:
            return {{x - 1, y}, Direction::EAST};
        default:
            ASSERT_NEVER_RUNS();
    }
}

void MouseInterface::moveForwardTo(const Coordinate& destinationTranslation, const Angle& destinationRotation) {

    /*

    // This function assumes that we're already facing the correct direction,
    // and that we simply need to move forward to reach the destination.

    // Determine delta between the two points
    Coordinate delta = destinationTranslation - m_mouse->getCurrentTranslation();
    Angle initialAngle = delta.getTheta();
    Distance previousDistance = delta.getRho();

    // Start the mouse moving forward
    m_mouse->setWheelSpeedsForMoveForward(m_wheelSpeedFraction);

    // Move forward until we've reached the destination
    do {
        // Assert that we're actually moving closer to the destination
        ASSERT_LE(delta.getRho().getMeters(), previousDistance.getMeters());
        previousDistance = delta.getRho();
        // Check if a stop has been requested
        BREAK_IF_STOPPED_ELSE_SLEEP_MIN();
        // Update the translation delta
        delta = destinationTranslation - m_mouse->getCurrentTranslation();
    }
    // While the angle delta is not ~180 degrees, sleep for a short amout of time
    while (std::abs((delta.getTheta() - initialAngle).getDegreesZeroTo360()) <  90
        || std::abs((delta.getTheta() - initialAngle).getDegreesZeroTo360()) > 270);

    // Stop the wheels and teleport to the exact destination
    m_mouse->stopAllWheels();

    */

    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::arcTo(const Coordinate& destinationTranslation, const Angle& destinationRotation,
        const Distance& radius, double extraWheelSpeedFraction) {

    /*
    // Determine the inital rotation delta in [-180, 180)
    Angle initialRotationDelta = getRotationDelta(m_mouse->getCurrentRotation(), destinationRotation);

    // Set the speed based on the initial rotation delta
    if (0 < initialRotationDelta.getDegreesUnbounded()) {
        m_mouse->setWheelSpeedsForCurveLeft(
            m_wheelSpeedFraction * extraWheelSpeedFraction, radius);
    }
    else {
        m_mouse->setWheelSpeedsForCurveRight(
            m_wheelSpeedFraction * extraWheelSpeedFraction, radius);
    }
    
    // While the deltas have the same sign, sleep for a short amount of time
    while (0 <
            initialRotationDelta.getRadiansUnbounded() *
            getRotationDelta(
                m_mouse->getCurrentRotation(),
                destinationRotation
            ).getRadiansUnbounded()) {
        BREAK_IF_STOPPED_ELSE_SLEEP_MIN();
    }

    // Stop the wheels and teleport to the exact destination
    m_mouse->stopAllWheels();
    */
    m_mouse->teleport(destinationTranslation, destinationRotation);
}

void MouseInterface::turnTo(const Coordinate& destinationTranslation, const Angle& destinationRotation) {
    // When we're turning in place, we set the wheels to half speed
    arcTo(destinationTranslation, destinationRotation, Distance::Meters(0), 0.5);
}

Angle MouseInterface::getRotationDelta(const Angle& from, const Angle& to) const {
    static const Angle lowerBound = Angle::Degrees(-180);
    static const Angle upperBound = Angle::Degrees(180);
    static const Angle fullCircle = Angle::Degrees(360);
    Angle delta = Angle::Radians(to.getRadiansZeroTo2pi() - from.getRadiansZeroTo2pi());
    if (delta.getRadiansUnbounded() < lowerBound.getRadiansUnbounded()) {
        delta += fullCircle;
    }
    if (upperBound.getRadiansUnbounded() <= delta.getRadiansUnbounded()) {
        delta -= fullCircle;
    }
    ASSERT_LE(lowerBound.getRadiansUnbounded(), delta.getRadiansUnbounded());
    ASSERT_LT(delta.getRadiansUnbounded(), upperBound.getRadiansUnbounded());
    return delta;
}

Coordinate MouseInterface::getCenterOfTile(int x, int y) const {
    ASSERT_TR(m_maze->withinMaze(x, y));
    static Distance tileLength = Distance::Meters(P()->wallLength() + P()->wallWidth());
    Coordinate centerOfTile = Coordinate::Cartesian(
        tileLength * (static_cast<double>(x) + 0.5),
        tileLength * (static_cast<double>(y) + 0.5)
    );
    return centerOfTile;
}

QPair<Coordinate, Angle> MouseInterface::getCrashLocation(
        QPair<int, int> currentTile, Direction destinationDirection) {

    static Distance halfWallLength = Distance::Meters(P()->wallLength() / 2.0);

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
    Coordinate centerOfTile = getCenterOfTile(currentTile.first, currentTile.second);
    Angle destinationRotation = DIRECTION_TO_ANGLE().value(destinationDirection);
    return {
        centerOfTile + Coordinate::Polar(halfWallLength, destinationRotation),
        destinationRotation
    };
}

} // namespace mms

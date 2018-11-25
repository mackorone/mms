#include "MouseInterface.h"

#include <QChar>
#include <QDebug>
#include <QPair>
#include <QtMath>

#include "units/Distance.h"
#include "units/Duration.h"

#include "Assert.h"
#include "Color.h"
#include "ColorManager.h"
#include "Dimensions.h"
#include "FontImage.h"
#include "Logging.h"
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
    m_movement(Movement::NONE),
    m_progress(0.0) {
}

QString MouseInterface::dispatch(const QString& command) {

    // TODO: upforgrabs
    // These functions should have sanity checks, e.g., correct
    // types, not finalizing static options more than once, etc.


    // TODO: MACK - set/unset wall? set and clear?

    static const QString ACK_STRING = "ACK";
    static const QString NO_ACK_STRING = "";
    static const QString ERROR_STRING = "!";

    QStringList tokens = command.split(" ", QString::SkipEmptyParts);
    QString function = tokens.at(0);

    // TODO: MACK - if it returns a response, it should be serial

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
        // TODO: MACK
        moveForward();
        return NO_ACK_STRING;
    }
    else if (function == "turnRight") {
        // TODO: MACK
        turnRight();
        return NO_ACK_STRING;
    }
    else if (function == "turnLeft") {
        // TODO: MACK
        turnLeft();
        return NO_ACK_STRING;
    }
    else if (function == "reset") {
        reset();
        return ACK_STRING;
    }
    else if (function == "setTileColor") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        QChar color = SimUtilities::strToChar(tokens.at(3));
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
        // TODO: MACK - don't allow empty string
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
        QChar direction = SimUtilities::strToChar(tokens.at(3));
        bool wallExists = SimUtilities::strToBool(tokens.at(4));
        if (wallExists) {
            setWall(x, y, direction);
        }
        return NO_ACK_STRING;
    }
    else if (function == "undeclareWall") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        QChar direction = SimUtilities::strToChar(tokens.at(3));
        clearWall(x, y, direction);
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
    if (isWallFront()) {
        // TODO: MACK
        return;
    }
    updateStartingLocationAndDirection();
    m_movement = Movement::MOVE_FORWARD;
}

void MouseInterface::turnRight() {
    updateStartingLocationAndDirection();
    m_movement = Movement::TURN_RIGHT;
}

void MouseInterface::turnLeft() {
    updateStartingLocationAndDirection();
    m_movement = Movement::TURN_LEFT;
}

void MouseInterface::reset() {
    m_mouse->reset();
}

void MouseInterface::setTileColor(int x, int y, QChar color) {

    if (!withinMaze(x, y)) {
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

    m_view->getMazeGraphic()->setTileColor(x, y, CHAR_TO_COLOR().value(color));
    m_tilesWithColor.insert({x, y});
}

void MouseInterface::clearTileColor(int x, int y) {

    if (!withinMaze(x, y)) {
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

    if (!withinMaze(x, y)) {
        qWarning().noquote().nospace()
            << "There is no tile at position (" << x << ", " << y << "), and"
            << " thus you cannot set its text to \"" << text << "\".";
        return;
    }

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

void MouseInterface::clearTileText(int x, int y) {

    if (!withinMaze(x, y)) {
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

void MouseInterface::setWall(int x, int y, QChar direction) {

    bool wallExists = true;

    if (!withinMaze(x, y)) {
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

void MouseInterface::clearWall(int x, int y, QChar direction) {

    if (!withinMaze(x, y)) {
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

void MouseInterface::updateStartingLocationAndDirection() {
    m_startingLocation = m_mouse->getCurrentDiscretizedTranslation();
    m_startingDirection = m_mouse->getCurrentDiscretizedRotation();
}

double MouseInterface::progressRequired(Movement movement) {
    switch (movement) {
        case Movement::MOVE_FORWARD:
            return 100.0;
        case Movement::TURN_RIGHT:
        case Movement::TURN_LEFT:
            return 33.33;
        default:
            ASSERT_NEVER_RUNS();
    }
}

void MouseInterface::clearTileColorImpl(int x, int y) {
    m_view->getMazeGraphic()->setTileColor(
        x, y, ColorManager::get()->getTileBaseColor());
}

void MouseInterface::clearTileTextImpl(int x, int y) {
    m_view->getMazeGraphic()->setTileText(x, y, {});
}

bool MouseInterface::isWall(QPair<QPair<int, int>, Direction> wall) const {
    int x = wall.first.first;
    int y = wall.first.second;
    Direction direction = wall.second;
    ASSERT_TR(withinMaze(x, y));
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

bool MouseInterface::isMoving() {
    return m_movement != Movement::NONE;
}

double MouseInterface::progressRemaining() {
    return progressRequired(m_movement) - m_progress;
}

void MouseInterface::moveALittle(double progress) {

    QPair<int, int> destinationLocation = m_startingLocation;
    Angle startingRotation = DIRECTION_TO_ANGLE().value(m_startingDirection);
    Angle destinationRotation = startingRotation;

    if (m_movement == Movement::MOVE_FORWARD) {
        if (m_startingDirection == Direction::NORTH) {
            destinationLocation.second += 1;
        }
        else if (m_startingDirection == Direction::EAST) {
            destinationLocation.first += 1;
        }
        else if (m_startingDirection == Direction::SOUTH) {
            destinationLocation.second -= 1;
        }
        else if (m_startingDirection == Direction::WEST) {
            destinationLocation.first -= 1;
        }
        else {
            ASSERT_NEVER_RUNS();
        }
    }
    else if (m_movement == Movement::TURN_RIGHT) {
        destinationRotation -= Angle::Degrees(90);
    }
    else if (m_movement == Movement::TURN_LEFT) {
        destinationRotation += Angle::Degrees(90);
    }
    else {
        ASSERT_NEVER_RUNS();
    }

    m_progress += progress;

    double required = progressRequired(m_movement);
    double remaining = progressRemaining();

    if (remaining < 0.0) {
        remaining = 0.0;
    }

    double two = 1.0 - (remaining / required);
    double one = 1.0 - two;


    // TODO: MACK
    Coordinate start =
        getCenterOfTile(m_startingLocation.first, m_startingLocation.second);
    Coordinate end =
        getCenterOfTile(destinationLocation.first, destinationLocation.second);

    m_mouse->teleport(
        start * one + end * two,
        startingRotation * one + destinationRotation * two
    );

    if (remaining == 0.0) {
        m_movement = Movement::NONE;
        m_progress = 0.0;
    }
}

bool MouseInterface::withinMaze(int x, int y) const {
    return (
        0 <= x && x < m_maze->getWidth() &&
        0 <= y && y < m_maze->getHeight()
    );
}

Coordinate MouseInterface::getCenterOfTile(int x, int y) const {
    ASSERT_TR(withinMaze(x, y));
    Coordinate centerOfTile = Coordinate::Cartesian(
        Dimensions::tileLength() * (static_cast<double>(x) + 0.5),
        Dimensions::tileLength() * (static_cast<double>(y) + 0.5)
    );
    return centerOfTile;
}

} 

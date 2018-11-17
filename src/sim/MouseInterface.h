#pragma once

#include <QMap>
#include <QObject>
#include <QPair>
#include <QSet>

#include "MazeView.h"
#include "Mouse.h"
#include "Param.h"

namespace mms {

enum class Movement {
    MOVE_FORWARD,
    TURN_RIGHT,
    TURN_LEFT,
};

class MouseInterface : public QObject {

    Q_OBJECT

public:

    MouseInterface(
        const Maze* maze,
        Mouse* mouse,
        MazeView* view);

    // Called when the algo started successfully
    void emitMouseAlgoStarted();

    // Execute a request, return a response
    QString dispatch(const QString& command);

    // A user pressed an input button in the UI
    void inputButtonWasPressed(int button);

    // TODO: MACK
    bool isMoving();
    double fracRemaining();
    void moveALittle(double frac);

signals:

    // Emit sanitized algorithm output
    void algoOutput(QString output);

    // An algorithm acknowledged an input button
    void inputButtonWasAcknowledged(int button);

    // The algorithm started as expected
    void mouseAlgoStarted();

private:

    // *********************** START PUBLIC INTERFACE ******************** //

    // ----- Any interface methods ----- //

    // Maze dimensions
    int getMazeWidth();
    int getMazeHeight();

    // Wall existence
    bool isWallFront();
    bool isWallRight();
    bool isWallLeft();

    // Movement
    void moveForward();
    void turnRight();
    void turnLeft();
    void reset();

    // Tile color
    void setTileColor(int x, int y, char color);
    void clearTileColor(int x, int y);
    void clearAllTileColor();

    // Tile text
    void setTileText(int x, int y, const QString& text);
    void clearTileText(int x, int y);
    void clearAllTileText();

    // Tile walls
    void declareWall(int x, int y, char direction, bool wallExists);
    void undeclareWall(int x, int y, char direction);

    // Input buttons
    bool wasInputButtonPressed(int inputButton);
    void acknowledgeInputButtonPressed(int inputButton);

    // ************************ END PUBLIC INTERFACE ********************* //

    // Pointers to various simulator objects
    const Maze* m_maze;
    Mouse* m_mouse;
    MazeView* m_view;

    // Whether or a stop was requested
    // TODO: MACK - delete this
    bool m_stopRequested;

    // TODO: MACK

    QPair<int, int> m_startingLocation;
    Direction m_startingDirection;

    QPair<int, int> m_destinationLocation;
    Direction m_destinationDirection;

    Movement m_movement;
    bool m_isMoving;
    double m_movementFraction;

    Coordinate m_destinationTranslation;
    Angle m_destinationRotation;

    // Whether or not the input buttons are pressed/acknowleged
    QMap<int, bool> m_inputButtonsPressed;

    // Whether or not the mouse has moved out the origin
    bool m_inOrigin;

    // A constant used to ensure that the mouse
    // doesn't travel too fast in DISCRETE mode
    double m_wheelSpeedFraction;

    // Cache of tiles, for making clearAll methods faster
    QSet<QPair<int, int>> m_tilesWithColor;
    QSet<QPair<int, int>> m_tilesWithText;

    // Implementation methods
    void setTileColorImpl(int x, int y, char color);
    void clearTileColorImpl(int x, int y);
    void setTileTextImpl(int x, int y, const QString& text);
    void clearTileTextImpl(int x, int y);
    void moveForwardImpl();
    void turnLeftImpl();
    void turnRightImpl();

    // Helper methods for wall retrieval and declaration
    bool isWall(QPair<QPair<int, int>, Direction> wall);
    bool hasOpposingWall(int x, int y, Direction d) const;
    QPair<QPair<int, int>, Direction> getOpposingWall(
        int x, int y, Direction d) const;

    // Some helper abstractions for mouse movements
    void moveForwardTo(const Coordinate& destinationTranslation, const Angle& destinationRotation);
    void arcTo(const Coordinate& destinationTranslation, const Angle& destinationRotation,
        const Distance& radius, double extraWheelSpeedFraction);
    void turnTo(const Coordinate& destinationTranslation, const Angle& destinationRotation);

    // Returns the angle with from "from" to "to", with values in [-180, 180) degrees
    Angle getRotationDelta(const Angle& from, const Angle& to) const;

    // Returns the center of a given tile
    Coordinate getCenterOfTile(int x, int y) const;

    // Returns the location of where the mouse should stop if it crashes
    QPair<Coordinate, Angle> getCrashLocation(
        QPair<int, int> currentTile, Direction destinationDirection);

};

} // namespace mms

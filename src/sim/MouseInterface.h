#pragma once

#include <QMap>
#include <QObject>
#include <QPair>
#include <QSet>

#include "MazeView.h"
#include "Mouse.h"

namespace mms {

enum class Movement {
    MOVE_FORWARD,
    TURN_RIGHT,
    TURN_LEFT,
    NONE,
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
    double progressRemaining();
    void moveALittle(double progress);

signals:

    // An algorithm acknowledged an input button
    void inputButtonWasAcknowledged(int button);

private:

    // *********************** START PUBLIC INTERFACE ******************** //

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

    // State used for asynchronous mouse movement
    void updateStartingLocationAndDirection();
    QPair<int, int> m_startingLocation;
    Direction m_startingDirection;
    Movement m_movement;
    double m_progress;
    double progressRequired(Movement movement);

    // Whether or not the input buttons are pressed/acknowleged
    QMap<int, bool> m_inputButtonsPressed;

    // Cache of tiles, for making clearAll methods faster
    QSet<QPair<int, int>> m_tilesWithColor;
    QSet<QPair<int, int>> m_tilesWithText;
    void clearTileColorImpl(int x, int y);
    void clearTileTextImpl(int x, int y);

    // Helper methods for wall retrieval and declaration
    bool isWall(QPair<QPair<int, int>, Direction> wall) const;
    bool hasOpposingWall(int x, int y, Direction d) const;
    QPair<QPair<int, int>, Direction> getOpposingWall(
        int x, int y, Direction d) const;

    // Returns the center of a given tile
    Coordinate getCenterOfTile(int x, int y) const;
};

} 

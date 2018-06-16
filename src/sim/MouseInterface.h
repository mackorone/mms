#pragma once

#include <QMap>
#include <QObject>
#include <QPair>
#include <QSet>

#include "DynamicMouseAlgorithmOptions.h"
#include "InterfaceType.h"
#include "MazeView.h"
#include "Mouse.h"
#include "Param.h"

namespace mms {

class MouseInterface : public QObject {

    Q_OBJECT

public:

    MouseInterface(
        const Maze* maze,
        Mouse* mouse,
        MazeView* view);

    // Called when the algo started successfully
    void emitMouseAlgoStarted();

    // Called when the algo process could not start
    void emitMouseAlgoCannotStart(QString string);

    // Execute a request, return a response
    QString dispatch(const QString& command);

    // Request that the mouse algorithm exit
    void requestStop();

    // A user pressed an input button in the UI
    void inputButtonWasPressed(int button);

    // Parameters set by the algorithm
    InterfaceType getInterfaceType(bool canFinalize) const;
    DynamicMouseAlgorithmOptions getDynamicOptions() const;

signals:

    // Emit sanitized algorithm output
    void algoOutput(QString output);

    // An algorithm acknowledged an input button
    void inputButtonWasAcknowledged(int button);

    // The algorithm started as expected
    void mouseAlgoStarted();

    // The algorithm could not be started
    void mouseAlgoCannotStart(QString errorString);

private:

    // *********************** START PUBLIC INTERFACE ******************** //

    // ----- Any interface methods ----- //

    // Config-related functions
    char getStartedDirection();
    void setStartingDirection(char direction);
    void setWheelSpeedFraction(double wheelSpeedFraction);

    // Misc functions
    double getRandom();
    int millis(); // # of milliseconds of sim time (adjusted based on sim speed) that have passed
    void delay(int milliseconds); // # of milliseconds of sim time (adjusted based on sim speed)

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

    // Tile fog
    void setTileFogginess(int x, int y, bool foggy);

    // Tile distance, where a negative distance corresponds to inf distance
    void declareTileDistance(int x, int y, int distance);
    void undeclareTileDistance(int x, int y);

    // Reset position of the mouse
    void resetPosition();

    // Input buttons
    bool inputButtonPressed(int inputButton);
    void acknowledgeInputButtonPressed(int inputButton);

    // ----- Any discrete interface methods ----- //

    bool wallFront();
    bool wallRight();
    bool wallLeft();

    void moveForward();
    void turnLeft();
    void turnRight();

    // ************************ END PUBLIC INTERFACE ********************* //

    // Pointers to various simulator objects
    const Maze* m_maze;
    Mouse* m_mouse;
    MazeView* m_view;

    // The interface type (DISCRETE or CONTINUOUS)
    InterfaceType m_interfaceType;
    mutable bool m_interfaceTypeFinalized;

    // The runtime algorithm options
    DynamicMouseAlgorithmOptions m_dynamicOptions;

    // Whether or a stop was requested
    bool m_stopRequested;

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

    // Helper methods for checking particular conditions and failing hard
    void ensureDiscreteInterface(const QString& callingFunction) const;
    void ensureContinuousInterface(const QString& callingFunction) const;
    void ensureAllowOmniscience(const QString& callingFunction) const;
    void ensureNotTileEdgeMovements(const QString& callingFunction) const;
    void ensureUseTileEdgeMovements(const QString& callingFunction) const;
    void ensureInsideOrigin(const QString& callingFunction) const;
    void ensureOutsideOrigin(const QString& callingFunction) const;

    // Implementation methods:
    // Any functionality that is executed as part of another MouseInterface
    // method should have an Impl method, and the Impl method should be called.
    // As an important note, if any methods can cause a crash, it must handle
    // the crash appropriately (see moveForwardImpl() for an example).
    void setTileColorImpl(int x, int y, char color);
    void clearTileColorImpl(int x, int y);
    void setTileTextImpl(int x, int y, const QString& text);
    void clearTileTextImpl(int x, int y);
    void declareWallImpl(
        QPair<QPair<int, int>, Direction> wall, bool wallExists, bool declareBothWallHalves);
    void undeclareWallImpl(
        QPair<QPair<int, int>, Direction> wall, bool declareBothWallHalves);
    bool wallFrontImpl(bool declareWallOnRead, bool declareBothWallHalves);
    bool wallLeftImpl(bool declareWallOnRead, bool declareBothWallHalves);
    bool wallRightImpl(bool declareWallOnRead, bool declareBothWallHalves);
    void moveForwardImpl(bool originMoveForwardToEdge = false);
    void turnLeftImpl();
    void turnRightImpl();
    void turnAroundLeftImpl();
    void turnAroundRightImpl();
    void turnToEdgeImpl(bool turnLeft);
    void turnAroundToEdgeImpl(bool turnLeft);

    // Helper methods for wall retrieval and declaration
    bool isWall(
        QPair<QPair<int, int>, Direction> wall,
        bool declareWallOnRead,
        bool declareBothWallHalves);
    bool hasOpposingWall(QPair<QPair<int, int>, Direction> wall) const;
    QPair<QPair<int, int>, Direction> getOpposingWall(
        QPair<QPair<int, int>, Direction> wall) const;

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

    // TODO: MACK - rename to Impl
    void doDiagonal(int count, bool startLeft, bool endLeft);

};

} // namespace mms

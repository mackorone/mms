#pragma once

#include "InterfaceType.h"
#include "MazeGraphic.h"
#include "Mouse.h"
#include "StaticMouseAlgorithmOptions.h"
#include "Param.h"

#define ENSURE_DISCRETE_INTERFACE ensureDiscreteInterface(__func__);
#define ENSURE_CONTINUOUS_INTERFACE ensureContinuousInterface(__func__);
#define ENSURE_ALLOW_OMNISCIENCE ensureAllowOmniscience(__func__);
#define ENSURE_NOT_TILE_EDGE_MOVEMENTS ensureNotTileEdgeMovements(__func__);
#define ENSURE_USE_TILE_EDGE_MOVEMENTS ensureUseTileEdgeMovements(__func__);
#define ENSURE_INSIDE_ORIGIN ensureInsideOrigin(__func__);
#define ENSURE_OUTSIDE_ORIGIN ensureOutsideOrigin(__func__);

// We have to forward declare the class (as opposed to including it) so as to
// avoid a circular dependency; IMouseAlgorithm.h already includes this file
class IMouseAlgorithm;

namespace sim {

class MouseInterface {

public:
    MouseInterface(
        const Maze* maze,
        Mouse* mouse,
        MazeGraphic* mazeGraphic,
        IMouseAlgorithm* mouseAlgorithm,
        std::set<char> allowableTileTextCharacters,
        StaticMouseAlgorithmOptions options);

    // ----- Any interface methods ----- //

    // Logging functions
    void debug(const std::string& str);
    void info(const std::string& str);
    void warn(const std::string& str);
    void error(const std::string& str);

    // Misc functions
    double getRandom();
    int millis(); // # of milliseconds of sim time (adjusted based on sim speed) that have passed
    void delay(int milliseconds); // # of milliseconds of sim time (adjusted based on sim speed)
    void quit();

    // Tile color
    void setTileColor(int x, int y, char color);
    void clearTileColor(int x, int y);
    void clearAllTileColor();

    // Tile text
    void setTileText(int x, int y, const std::string& text);
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

    // ----- Continuous interface methods ----- //

    // Get the magnitude of the max speed of any one wheel in rpm
    double getWheelMaxSpeed(const std::string& name);

    // Set the speed of any one wheel
    void setWheelSpeed(const std::string& name, double rpm);

    // Get the number of encoder ticks per revolution for a wheel
    double getWheelEncoderTicksPerRevolution(const std::string& name);

    // Read the encoder for a particular wheel
    int readWheelEncoder(const std::string& name);

    // Reset the encoder for a particular wheel to zero, but only if the encoder type is relative
    void resetWheelEncoder(const std::string& name);

    // Returns a value in [0.0, 1.0]
    double readSensor(std::string name);

    // Returns deg/s of rotation
    double readGyro();

    // ----- Any discrete interface methods ----- //

    bool wallFront();
    bool wallRight();
    bool wallLeft();

    // ----- Basic discrete interface methods ----- //

    void moveForward();
    void moveForward(int count);

    void turnLeft();
    void turnRight();

    void turnAroundLeft();
    void turnAroundRight();

    // ----- Special discrete interface methods ----- //

    void originMoveForwardToEdge();
    void originTurnLeftInPlace();
    void originTurnRightInPlace();

    void moveForwardToEdge();
    void moveForwardToEdge(int count);

    void turnLeftToEdge();
    void turnRightToEdge();

    void turnAroundLeftToEdge();
    void turnAroundRightToEdge();

    void diagonalLeftLeft(int count);
    void diagonalLeftRight(int count);
    void diagonalRightLeft(int count);
    void diagonalRightRight(int count);

    // ----- Omniscience methods ----- //

    int currentXTile();
    int currentYTile();
    char currentDirection();

    double currentXPosMeters();
    double currentYPosMeters();
    double currentRotationDegrees();

private:
    const Maze* m_maze;
    Mouse* m_mouse;
    MazeGraphic* m_mazeGraphic;
    IMouseAlgorithm* m_mouseAlgorithm;
    std::set<char> m_allowableTileTextCharacters;
    StaticMouseAlgorithmOptions m_options;

    // Whether or not the mouse has moved out the origin
    bool m_inOrigin;

    // Cache of tiles, for making clearAll methods faster
    std::set<std::pair<int, int>> m_tilesWithColor;
    std::set<std::pair<int, int>> m_tilesWithText;

    // Helper methods for checking particular conditions and failing hard
    void ensureDiscreteInterface(const std::string& callingFunction) const;
    void ensureContinuousInterface(const std::string& callingFunction) const;
    void ensureAllowOmniscience(const std::string& callingFunction) const;
    void ensureNotTileEdgeMovements(const std::string& callingFunction) const;
    void ensureUseTileEdgeMovements(const std::string& callingFunction) const;
    void ensureInsideOrigin(const std::string& callingFunction) const;
    void ensureOutsideOrigin(const std::string& callingFunction) const;

    // Implementation methods:
    // Any functionality that is executed as part of another MouseInterface
    // method should have an Impl method, and the Impl method should be called.
    // As an important note, if any methods can cause a crash, it must handle
    // the crash appropriately (see moveForwardImpl() for an example).
    void setTileColorImpl(int x, int y, char color);
    void clearTileColorImpl(int x, int y);
    void setTileTextImpl(int x, int y, const std::string& text);
    void clearTileTextImpl(int x, int y);
    void declareWallImpl(
        std::pair<std::pair<int, int>, Direction> wall, bool wallExists, bool declareBothWallHalves);
    void undeclareWallImpl(
        std::pair<std::pair<int, int>, Direction> wall, bool declareBothWallHalves);
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
    bool isWall(std::pair<std::pair<int, int>, Direction> wall, bool declareWallOnRead, bool declareBothWallHalves);
    bool hasOpposingWall(std::pair<std::pair<int, int>, Direction> wall) const;
    std::pair<std::pair<int, int>, Direction> getOpposingWall(
        std::pair<std::pair<int, int>, Direction> wall) const;

    // Some helper abstractions for mouse movements
    void moveForwardTo(const Cartesian& destinationTranslation, const Radians& destinationRotation);
    void arcTo(const Cartesian& destinationTranslation, const Radians& destinationRotation,
        const Meters& radius, double extraWheelSpeedFraction);
    void turnTo(const Cartesian& destinationTranslation, const Radians& destinationRotation);

    // Returns the angle with from "from" to "to", with values in [-180, 180) degrees
    Radians getRotationDelta(const Radians& from, const Radians& to) const;

    // Returns the center of a given tile
    Cartesian getCenterOfTile(int x, int y) const;

    // Returns the location of where the mouse should stop if it crashes
    std::pair<Cartesian, Degrees> getCrashLocation(
        std::pair<int, int> currentTile, Direction destinationDirection);

    // TODO: MACK - rename to Impl
    void doDiagonal(int count, bool startLeft, bool endLeft);

};

} // namespace sim

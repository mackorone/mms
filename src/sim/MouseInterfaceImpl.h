#pragma once

#include "MazeGraphic.h"
#include "Mouse.h"

namespace sim {

class MouseInterfaceImpl {

public:
    MouseInterfaceImpl(
        const Maze* maze,
        Mouse* mouse,
        MazeGraphic* mazeGraphic);

    // ----- Any interface methods ----- //

    void debug(const std::string& str);
    void info(const std::string& str);
    void warn(const std::string& str);
    void error(const std::string& str);
    double getRandom();
    int millis();
    void delay(int milliseconds);
    void quit();
    void setTileColor(int x, int y, char color);
    void clearTileColor(int x, int y);
    void clearAllTileColor();
    void setTileText(int x, int y, const std::string& text,
        int tileTextNumberOfRows, int tileTextNumberOfCols);
    void clearTileText(int x, int y);
    void clearAllTileText();
    void declareWall(
        std::pair<std::pair<int, int>, Direction> wall,
        bool wallExists, bool declareBothWallHalves);
    void undeclareWall(
        std::pair<std::pair<int, int>, Direction> wall,
        bool declareBothWallHalves);
    void setTileFogginess(int x, int y, bool foggy);
    void declareTileDistance(
        int x, int y, int distance,
        bool setTileTextWhenDistanceDeclared,
        bool setTileBaseColorWhenDistanceDeclaredCorrectly,
        int tileTextNumberOfRows, int tileTextNumberOfCols);
    void undeclareTileDistance(int x, int y,
        bool setTileTextWhenDistanceDeclared,
        bool setTileBaseColorWhenDistanceDeclaredCorrectly);
    void resetPosition();
    bool inputButtonPressed(int inputButton);
    void acknowledgeInputButtonPressed(int inputButton);

    // ----- Continuous interface methods ----- //

    double getWheelMaxSpeed(const std::string& name);
    void setWheelSpeed(const std::string& name, double rpm);
    double getWheelEncoderTicksPerRevolution(const std::string& name);
    int readWheelEncoder(const std::string& name);
    void resetWheelEncoder(const std::string& name);
    double readSensor(std::string name);
    double readGyro();

    // ----- Any discrete interface methods ----- //

    bool wallFront(bool declareWallOnRead, bool declareBothWallHalves);
    bool wallLeft(bool declareWallOnRead, bool declareBothWallHalves);
    bool wallRight(bool declareWallOnRead, bool declareBothWallHalves);

    // ----- Basic discrete interface methods ----- //

    void moveForward(double wheelSpeedFraction);
    void moveForward(int count, double wheelSpeedFraction);
    void turnLeft(double wheelSpeedFraction);
    void turnRight(double wheelSpeedFraction);
    void turnAroundLeft(double wheelSpeedFraction);
    void turnAroundRight(double wheelSpeedFraction);

    // ----- Edge discrete interface methods ----- //

    void originMoveForwardToEdge(double wheelSpeedFraction);
    void originTurnLeftInPlace(double wheelSpeedFraction);
    void originTurnRightInPlace(double wheelSpeedFraction);
    void moveForwardToEdge(double wheelSpeedFraction);
    void moveForwardToEdge(int count, double wheelSpeedFraction);
    void turnLeftToEdge(double wheelSpeedFraction);
    void turnRightToEdge(double wheelSpeedFraction);
    void turnAroundLeftToEdge(double wheelSpeedFraction);
    void turnAroundRightToEdge(double wheelSpeedFraction);
    void diagonalLeftLeft(int count, double wheelSpeedFraction);
    void diagonalLeftRight(int count, double wheelSpeedFraction);
    void diagonalRightLeft(int count, double wheelSpeedFraction);
    void diagonalRightRight(int count, double wheelSpeedFraction);

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

    // Cache of tiles, for making clearAll methods faster
    std::set<std::pair<int, int>> m_tilesWithColor;
    std::set<std::pair<int, int>> m_tilesWithText;

    // Helper methods for wall retrieval and declaration
    bool isWall(std::pair<std::pair<int, int>, Direction> wall, bool declareWallOnRead, bool declareBothWallHalves);
    bool hasOpposingWall(std::pair<std::pair<int, int>, Direction> wall) const;
    std::pair<std::pair<int, int>, Direction> getOpposingWall(
        std::pair<std::pair<int, int>, Direction> wall) const;

    // Helper methods for edge movements
    void curveTurn(bool turnLeft, double wheelSpeedFraction);
    void turnAroundToEdge(bool turnLeft, double wheelSpeedFraction);
    void doDiagonal(int count, bool startLeft, bool endLeft, double wheelSpeedFraction);

    // Some helper abstractions for mouse movements
    void moveForwardTo(const Cartesian& destinationTranslation, const Radians& destinationRotation,
        double wheelSpeedFraction);
    void arcTo(const Cartesian& destinationTranslation, const Radians& destinationRotation,
        const Meters& radius, double wheelSpeedFraction);
    void turnTo(const Cartesian& destinationTranslation, const Radians& destinationRotation,
        double wheelSpeedFraction);

    // Returns the angle with from "from" to "to", with values in [-180, 180) degrees
    Radians getRotationDelta(const Radians& from, const Radians& to) const;

    // Returns the center of a given tile
    Cartesian getCenterOfTile(int x, int y) const;
};

} // namespace sim

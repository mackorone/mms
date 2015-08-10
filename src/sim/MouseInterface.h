#pragma once

#include "InterfaceType.h"
#include "MazeGraphic.h"
#include "Mouse.h"
#include "Param.h"

#define ENSURE_DISCRETE_INTERFACE ensureDiscreteInterface(__func__);
#define ENSURE_CONTINUOUS_INTERFACE ensureContinuousInterface(__func__);

namespace manual {
    class Manual;
}

namespace sim {

class MouseInterface {

public:
    MouseInterface(const Maze* maze, Mouse* mouse, MazeGraphic* mazeGraphic);

    // Any interface methods
    // TODO: random???
    // TODO: logging??? NOPE print instead
    void delay(int milliseconds);

    void setTileColor(int x, int y, char color);
    void clearTileColor(int x, int y);
    void clearAllTileColor();

    void setTileDistance(int x, int y, int distance);
    void clearTileDistance(int x, int y);
    void clearAllTileDistance();

    void setTileFogginess(int x, int y, bool foggy);

    void declareWall(int x, int y, char direction, bool wallExists);
    void undeclareWall(int x, int y, char direction);

    void resetPosition();

    bool inputButtonPressed(int inputButton);
    void acknowledgeInputButtonPressed(int inputButton);

    // Continuous interface methods
    void setWheelSpeeds(double leftWheelRadiansPerSeconds, double rightWheelRadiansPerSecond);
    double read(std::string name);

    // Discrete interface methods
    bool wallFront();
    bool wallRight();
    bool wallLeft();
    void moveForward();
    void turnRight();
    void turnLeft();
    void turnAround();

private:
    const Maze* m_maze;
    Mouse* m_mouse;
    MazeGraphic* m_mazeGraphic;
    std::set<std::pair<int, int>> m_tilesWithColor;

    void ensureDiscreteInterface(const std::string& callingFunction) const;
    void ensureContinuousInterface(const std::string& callingFunction) const;

    std::pair<int, int> getDiscretizedTranslation() const;
    Direction getDiscretizedRotation() const;

    void prepareForLaunch();

    void checkPaused() const;
    bool isWall(std::pair<int, int> position, Direction direction);
    bool withinMaze(int x, int y) const;
    bool hasOpposingWall(int x, int y, Direction direction) const;
    std::pair<std::pair<int, int>, Direction> getOpposingWall(int x, int y, Direction direction) const;

    friend class manual::Manual;
};

} // namespace sim

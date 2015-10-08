#pragma once

#include "InterfaceType.h"
#include "MazeGraphic.h"
#include "Mouse.h"
#include "Param.h"

// Windows compatibility
#ifdef _WIN32
#define __func__ __FUNCTION__
#endif

#define ENSURE_DISCRETE_INTERFACE ensureDiscreteInterface(__func__);
#define ENSURE_CONTINUOUS_INTERFACE ensureContinuousInterface(__func__);

namespace manual {
    class Manual;
}

namespace sim {

class MouseInterface {

public:
    MouseInterface(const Maze* maze, Mouse* mouse, MazeGraphic* mazeGraphic);

    // ----- Any interface methods ----- //

    // TODO: random???
    // TODO: logging??? NOPE print (arduino) instead
    void delay(int milliseconds);

    void setTileColor(int x, int y, char color);
    void clearTileColor(int x, int y);
    void clearAllTileColor();

    void declareWall(int x, int y, char direction, bool wallExists);
    void undeclareWall(int x, int y, char direction);

    void setTileFogginess(int x, int y, bool foggy);
    // TODO: MACK - (un)declareTraversed (how should this work???)

    void declareTileDistance(int x, int y, int distance);
    void undeclareTileDistance(int x, int y);

    void resetPosition();

    bool inputButtonPressed(int inputButton);
    void acknowledgeInputButtonPressed(int inputButton);

    // ----- Continuous interface methods ----- //

    // Counterclockwise looking into the wheel is positive, clockwise looking into the wheel is negative
    void setWheelSpeeds(double leftWheelRadiansPerSeconds, double rightWheelRadiansPerSecond);

    // Returns a value in [0.0, 1.0]
    double read(std::string name);

    // Returns deg/s of rotation
    double readGyro();

    // ----- Discrete interface methods ----- //

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

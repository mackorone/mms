#pragma once

#include "InterfaceType.h"
#include "MazeGraphic.h"
#include "Mouse.h"
#include "Param.h"

#define ENSURE_INITIALIZED_MOUSE ensureInitializedMouse(__func__);
#define ENSURE_DECLARED_INTERFACE ensureDeclaredInterface(__func__);
#define ENSURE_DISCRETE_INTERFACE ensureDiscreteInterface(__func__);
#define ENSURE_CONTINUOUS_INTERFACE ensureContinuousInterface(__func__);

namespace sim {

class MouseInterface {

public:
    MouseInterface(const Maze* maze, Mouse* mouse, MazeGraphic* mazeGraphic);
    ~MouseInterface();

    // The mouse must be initialized before first and foremost. Afterwards, the interface type
    // must be declared. These must be done  before calling any other interface methods
    void initializeMouse(const std::string& mouseFile);
    void declareInterfaceType(InterfaceType interfaceType);

    // Any interface methods
    void delay(int milliseconds);

    void setTileColor(int x, int y, char color);
    void clearTileColor(int x, int y);
    void clearAllTileColor();

    void setTileText(int x, int y, const std::string& text);
    void clearTileText(int x, int y);
    void clearAllTileText();

    void declareWall(int x, int y, char direction, bool wallExists);
    void undeclareWall(int x, int y, char direction);
    void undeclareAllWalls();

    void resetPosition();
    bool inputButtonPressed(int inputButton);
    void acknowledgeInputButtonPressed(int inputButton);

    // Continuous interface methods
    void setWheelSpeeds(float leftWheelRadiansPerSeconds, float rightWheelRadiansPerSecond);
    float read(std::string name);

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

    void ensureInitializedMouse(const std::string& callingFunction) const;
    void ensureDeclaredInterface(const std::string& callingFunction) const;
    void ensureDiscreteInterface(const std::string& callingFunction) const;
    void ensureContinuousInterface(const std::string& callingFunction) const;

    std::pair<int, int> getDiscretizedTranslation() const;
    Direction getDiscretizedRotation() const;

    void checkPaused();
    bool isWall(std::pair<int, int> position, Direction direction);
};

} // namespace sim

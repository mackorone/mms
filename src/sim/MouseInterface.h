#pragma once

#include "InterfaceType.h"
#include "MazeGraphic.h"
#include "Mouse.h"
#include "Param.h"

#define ENSURE_DECLARED ensureDeclaredInterface(__func__);
#define ENSURE_DISCRETE ensureDiscreteInterface(__func__);
#define ENSURE_CONTINUOUS ensureContinuousInterface(__func__);

namespace sim {

class MouseInterface {

public:
    MouseInterface(const Maze* maze, Mouse* mouse, MazeGraphic* mazeGraphic);
    ~MouseInterface();

    // The interface type must be declared before calling any other interface methods
    void declareInterfaceType(InterfaceType interfaceType);

    // Any interface methods
    void delay(int milliseconds);
    void colorTile(int x, int y, char color);
    void declareWall(int x, int y, char direction, bool isWall);
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
    // TODO: Other sensor readings and such

private:
    const Maze* m_maze;
    Mouse* m_mouse;
    MazeGraphic* m_mazeGraphic;

    void ensureDeclaredInterface(const std::string& callingFunction) const;
    void ensureDiscreteInterface(const std::string& callingFunction) const;
    void ensureContinuousInterface(const std::string& callingFunction) const;

    std::pair<int, int> getDiscretizedTranslation() const;
    Direction getDiscretizedRotation() const;

    void checkPaused();
    bool isWall(std::pair<int, int> position, Direction direction) const;
};

} // namespace sim

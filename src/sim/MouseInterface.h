#pragma once

#include "InterfaceType.h"
#include "Mouse.h"
#include "Param.h"

#define ENSURE_DISCRETE ensureDiscreteInterface(__func__);
#define ENSURE_CONTINUOUS ensureContinuousInterface(__func__);

namespace sim {

class MouseInterface {

public:
    MouseInterface(const Maze* maze, Mouse* mouse);
    ~MouseInterface();

    // The interface type must be declared before calling any other interface methods
    void declareInterfaceType(InterfaceType interfaceType);

    // Any interface methods
    void delay(int milliseconds);

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

    // ---------------------- Mouse Interface Methods ----------------------- //

    // Discrete interface
    bool undoRequested();
    bool resetRequested();
    void undoHonored();
    void resetHonored();
    void resetPosition(); // Sets the position of the mouse to (0,0)

    // ------ Simulation utilities (not applicable to the real mouse) ------- //

    void resetColors(int curX, int curY); // Resets the tile colors of the maze

private:
    const Maze* m_maze;
    Mouse* m_mouse;

    void ensureDiscreteInterface(const std::string& callingFunction);
    void ensureContinuousInterface(const std::string& callingFunction);
};

} // namespace sim

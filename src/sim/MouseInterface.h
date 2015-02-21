#pragma once

#include "Param.h"
#include "Mouse.h"

namespace sim {

class MouseInterface {

public:
    MouseInterface(Mouse* mouse);
    ~MouseInterface();

    // TODO: List of sensor interfaces
    // TODO: List of wheel interfaces

    void setLeftWheelSpeed(float radiansPerSecond); 
    void setRightWheelSpeed(float radiansPerSecond); 

    // ---------------------- Mouse Interfact Methods ----------------------- //

    bool wallFront();
    bool wallRight();
    bool wallLeft();
    void moveForward();
    void turnRight();
    void turnLeft();
    void turnAround();

    bool undoRequested();
    bool resetRequested();
    void undoHonored();
    void resetHonored();

    void resetPosition(); // Sets the position of the mouse to (0,0)


    // ------ Simulation utilities (not applicable to the real mouse) ------- //

    void resetColors(int curX, int curY); // Resets the tile colors of the maze

private:
    Mouse* m_mouse;
};

} // namespace sim

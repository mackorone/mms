#pragma once

namespace mack {

class MouseInterface {

public:
    MouseInterface();

    // Button inputs
    bool inputButtonPressed(int inputButton);
    void acknowledgeInputButtonPressed(int inputButton);

    // Sensor inputs
    bool wallFront();
    bool wallRight();
    bool wallLeft();
    
    // Movements
    void moveForward();
    void turnRight();
    void turnLeft();
    void turnAround();

};

} // namespace mack

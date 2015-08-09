#pragma once

#include "../IMouseAlgorithm.h"

namespace continuous {

class Continuous : public IMouseAlgorithm {

public:
    std::string mouseFile() const;
    sim::InterfaceType interfaceType() const;
    void solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse);

private:
    bool wallRight();
    bool wallFront();
    bool wallLeft();
    void turnRight();
    void turnLeft();
    void moveForward();
    void correctErrors();

    sim::MouseInterface* m_mouse;

};

} // namespace continuous

#pragma once

#include "Constants.h"
#include "Mouse.h"

namespace sim {

class MouseGraphic {

public:
    MouseGraphic(Mouse* mouse);
    ~MouseGraphic();
    void draw();
    void drawDirection(float c1X, float c1Y, float c2X, float c2Y);

private:
    Mouse* m_mouse;
    GLfloat* m_color;
};

} // namespace sim

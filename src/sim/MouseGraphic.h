#pragma once

#include "Colors.h"
#include "Mouse.h"

namespace sim {

class MouseGraphic {

public:
    MouseGraphic(Mouse* mouse);
    void draw();

private:
    Mouse* m_mouse;
    GLfloat* m_color;
};

} // namespace sim

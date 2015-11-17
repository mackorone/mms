#pragma once

#include "Color.h"
#include "Mouse.h"

namespace sim {

class MouseGraphic {

public:
    MouseGraphic(const Mouse* mouse);
    void draw();

private:
    const Mouse* m_mouse;
};

} // namespace sim

#pragma once

#include "Colors.h"
#include "Mouse.h"

namespace sim {

class MouseGraphic {

public:
    MouseGraphic(const Mouse* mouse);
    void draw();

    // TODO: get number of TriangleGraphic

private:
    const Mouse* m_mouse;
};

} // namespace sim

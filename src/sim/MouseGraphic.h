#pragma once

#include "Colors.h"
#include "Mouse.h"

namespace sim {

class MouseGraphic {

public:
    MouseGraphic(const Mouse* mouse);
    void draw();

    // TODO
    // TODO: get number of TriangleGraphic
    // TODO: Keep a list of all of the polygons, write to the buffer, no draw method...
    int trianglesPerMouse();

private:
    const Mouse* m_mouse;
};

} // namespace sim

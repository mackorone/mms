#pragma once

#include "Color.h"
#include "Mouse.h"

namespace sim {

class View; // TODO: MACK

class MouseGraphic {

public:
    MouseGraphic(const Mouse* mouse, View* view); // TODO: MACK
    void draw(const Coordinate& currentTranslation, const Angle& currentRotation) const;

private:
    const Mouse* m_mouse;
    View* m_view; // TODO: MACK
};

} // namespace sim

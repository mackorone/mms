#pragma once

#include "Color.h"
#include "Mouse.h"

namespace sim {

class MouseGraphic {

public:
    MouseGraphic(const Mouse* mouse);
    void draw(const Coordinate& currentTranslation, const Angle& currentRotation) const;

private:
    const Mouse* m_mouse;
};

} // namespace sim

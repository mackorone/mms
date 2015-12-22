#pragma once

#include "Color.h"
#include "Mouse.h"
#include "ViewData.h"

namespace sim {

class MouseGraphic {

public:
    MouseGraphic(const Mouse* mouse, ViewData* data);
    void draw(const Coordinate& currentTranslation, const Angle& currentRotation) const;

private:
    const Mouse* m_mouse;
    ViewData* m_data; // TODO: MACK
};

} // namespace sim

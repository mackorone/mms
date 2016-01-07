#pragma once

#include "BufferInterface.h"
#include "Color.h"
#include "Mouse.h"

namespace sim {

class MouseGraphic {

public:
    MouseGraphic(const Mouse* mouse, BufferInterface* bufferInterface);
    void draw(const Coordinate& currentTranslation, const Angle& currentRotation) const;

private:
    const Mouse* m_mouse;
    BufferInterface* m_bufferInterface;

};

} // namespace sim

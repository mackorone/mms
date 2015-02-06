#include "Mouse.h"

#include <cmath>
#include <iostream>

#include "Constants.h"
#include "Utilities.h"

namespace sim {

Mouse::Mouse() : m_translation(Cartesian(0, 0)), m_rotation(Radians(0)),
        m_rightWheel(Meters(0.05), Meters(0.01), Cartesian(0.0, 0.0)),
        m_leftWheel(Meters(0.05), Meters(0.01), Cartesian(0.0, 0.0)) {

    // Create the vertices for the mouse
    std::vector<Cartesian> vertices;
    vertices.push_back(Cartesian(0.06, 0.06));
    vertices.push_back(Cartesian(0.06, P()->wallLength() + P()->wallWidth() - 0.06));
    vertices.push_back(Cartesian(P()->wallLength() + P()->wallWidth() - 0.06, P()->wallLength() + P()->wallWidth() - 0.06));
    vertices.push_back(Cartesian(P()->wallLength() + P()->wallWidth() - 0.06, 0.06));
    m_shape.setVertices(vertices);
}

Cartesian Mouse::getTranslation() const {
    return m_translation;
}

Radians Mouse::getRotation() const {
    return m_rotation;
}

Polygon Mouse::getShape() const {
    return m_shape;
}

} // namespace sim

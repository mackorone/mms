#include "Mouse.h"

#include <cmath>
#include <iostream>

#include "Constants.h"
#include "Utilities.h"

namespace sim {

Mouse::Mouse() : m_translation(Cartesian(0, 0)), m_rotation(Radians(0)) {

    // Create the vertices for the mouse
    std::vector<Cartesian> vertices;
    vertices.push_back(Cartesian(0.05, 0.05));
    vertices.push_back(Cartesian(0.05, P()->wallLength() + P()->wallWidth() - 0.05));
    vertices.push_back(Cartesian(P()->wallLength() + P()->wallWidth() - 0.05, P()->wallLength() + P()->wallWidth() - 0.05));
    vertices.push_back(Cartesian(P()->wallLength() + P()->wallWidth() - 0.05, 0.05));
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

#include "Polygon.h"

namespace sim {

Polygon::Polygon() {
}

std::vector<Cartesian> Polygon::getVertices() const {
    return m_vertices;
}

void Polygon::setVertices(const std::vector<Cartesian>& vertices) {
    // TODO: Sort the vertices so that they're in order???
    m_vertices = vertices;
}

} // namespace sim

#include "Polygon.h"

#include "units/Polar.h"

namespace sim {

Polygon::Polygon() {
}

std::vector<Cartesian> Polygon::getVertices() const {
    return m_vertices;
}

// TODO: Ugly
void Polygon::setVertices(const std::vector<Cartesian>& vertices) {
    // TODO: Sort the vertices so that they're in order???
    m_vertices = vertices;
}

Polygon Polygon::translate(const Coordinate& translation) {
    std::vector<Cartesian> vertices;
    for (int i = 0; i < m_vertices.size(); i += 1) {
        vertices.push_back(m_vertices.at(i) + translation);
    }
    // TODO
    Polygon p;
    p.setVertices(vertices);
    return p;
}

Polygon Polygon::rotate(const Angle& rotation) {
    std::vector<Cartesian> vertices;
    for (int i = 0; i < m_vertices.size(); i += 1) {
        Polar v(m_vertices.at(i));
        // TODO: Fix this
        vertices.push_back(Polar(v.getRho(), v.getTheta() + rotation.getRadians()));
    }
    Polygon p;
    p.setVertices(vertices);
    return p;
}

} // namespace sim

#include "Polygon.h"

#include <Polar.h>

namespace sim {

Polygon::Polygon() {
}

Polygon::Polygon(const Polygon& polygon) {
    m_vertices = polygon.getVertices();
}

Polygon::Polygon(const std::vector<Cartesian>& vertices) {
    m_vertices = vertices;
}

std::vector<Cartesian> Polygon::getVertices() const {
    return m_vertices;
}

Polygon Polygon::translate(const Coordinate& translation) const {
    std::vector<Cartesian> vertices;
    for (Cartesian vertex : m_vertices) {
        vertices.push_back(vertex + translation);
    }
    return Polygon(vertices);
}

Polygon Polygon::rotateAroundPoint(const Angle& angle, const Coordinate& point) const {
    std::vector<Cartesian> vertices;
    for (Cartesian vertex : m_vertices) {
        Cartesian relative(vertex.getX() - point.getX(), vertex.getY() - point.getY());
        Polar rotated(relative.getRho(), relative.getTheta() + angle);
        Cartesian absolute(rotated.getX() + point.getX(), rotated.getY() + point.getY());
        vertices.push_back(absolute);
    }
    return Polygon(vertices);
}

} // namespace sim

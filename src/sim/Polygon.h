#pragma once

#include <vector>

#include <Angle.h>
#include <Cartesian.h>

namespace sim {

class Polygon {

public:
    Polygon();
    Polygon(const Polygon& polygon);
    Polygon(const std::vector<Cartesian>& vertices);
    std::vector<Cartesian> getVertices() const;
    Polygon translate(const Coordinate& translation) const;
    Polygon rotateAroundPoint(const Angle& angle, const Coordinate& point) const;

    // TODO: triangulate the polygon when created. adjust the polygon values as well

private:
    std::vector<Cartesian> m_vertices;
    std::vector<std::vector<Cartesian>> m_triangles; // TODO... hmmm
};

} // namespace sim

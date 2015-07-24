#pragma once

#include <vector>

#include "units/Angle.h"
#include "units/Cartesian.h"

#include "Triangle.h"

namespace sim {

class Polygon {

public:
    Polygon();
    Polygon(const Polygon& polygon);
    Polygon(const std::vector<Cartesian>& vertices);

    std::vector<Cartesian> getVertices() const;
    std::vector<std::pair<Cartesian, Cartesian>> getLineSegments() const;

    MetersSquared area() const;
    Cartesian centroid() const;

    Polygon translate(const Coordinate& translation) const;
    Polygon rotateAroundPoint(const Angle& angle, const Coordinate& point) const;
    std::vector<Triangle> triangulate() const;

private:
    std::vector<Cartesian> m_vertices;
};

} // namespace sim

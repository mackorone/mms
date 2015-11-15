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

    static Polygon createCirclePolygon(const Cartesian& position, const Distance& radius, int numberOfEdges = 8);

    std::vector<Cartesian> getVertices() const;
    std::vector<Triangle> getTriangles() const;
    std::vector<std::pair<Cartesian, Cartesian>> getLineSegments() const;

    MetersSquared area() const;
    Cartesian centroid() const;

    Polygon translate(const Coordinate& translation) const;
    Polygon rotateAroundPoint(const Angle& angle, const Coordinate& point) const;

private:
    std::vector<Cartesian> m_vertices;
    std::vector<Triangle> m_triangles;

    std::vector<Triangle> triangulate() const;
};

} // namespace sim

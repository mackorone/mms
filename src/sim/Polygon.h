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
    std::vector<Triangle> getTriangles() const;
    std::vector<std::pair<Cartesian, Cartesian>> getLineSegments() const;

    MetersSquared area() const;
    Cartesian centroid() const;

    Polygon translate(const Coordinate& translation) const;
    Polygon rotateAroundPoint(const Angle& angle, const Coordinate& point) const;

private:
    std::vector<Cartesian> m_vertices;
    std::vector<Triangle> m_triangles;

    // This special constructor makes it so that rotate and translate don't
    // require re-triangulation. We keep it private since it's pretty easy to
    // abuse the fact that the triangles argument should be the triangulation
    // of the polygon specified by the vertices argument.
    Polygon(const std::vector<Cartesian>& vertices, const std::vector<Triangle>& triangles);

    static std::vector<Triangle> triangulate(const std::vector<Cartesian>& vertices);
};

} // namespace sim

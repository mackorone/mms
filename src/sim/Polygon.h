#pragma once

#include <vector>

#include "units/Angle.h"
#include "units/Cartesian.h"

namespace sim {

class Polygon {

public:
    Polygon();
    Polygon(const Polygon& polygon);
    Polygon(const std::vector<Cartesian>& vertices);
    std::vector<Cartesian> getVertices() const;
    Polygon translate(const Coordinate& translation) const;
    Polygon rotateAroundPoint(const Angle& angle, const Coordinate& point) const;

private:
    std::vector<Cartesian> m_vertices;
};

} // namespace sim

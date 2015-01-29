#pragma once

#include <vector>

#include "units/Angle.h"
#include "units/Cartesian.h"

namespace sim {

class Polygon {

public:
    Polygon();
    std::vector<Cartesian> getVertices() const;
    void setVertices(const std::vector<Cartesian>& vertices); // TODO: Ugly
    Polygon translate(const Coordinate& translation);
    Polygon rotate(const Angle& rotation);

private:
    std::vector<Cartesian> m_vertices;
};

} // namespace sim

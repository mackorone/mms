#pragma once

#include <vector>

#include "units/Angle.h"
#include "units/Cartesian.h"

namespace sim {

// A Polygon is a rigid body within the simulator
class Polygon {

// TODO: Mack will fix this mess

public:
    Polygon();
    std::vector<Cartesian> getVertices() const;
    void setVertices(const std::vector<Cartesian>& vertices); // TODO: Ugly
    Polygon translate(const Coordinate& translation) const;
    Polygon rotate(const Angle& rotation) const;

private:
    std::vector<Cartesian> m_vertices;
};

} // namespace sim

#pragma once

#include <vector>

#include "units/Cartesian.h"

namespace sim {

class Polygon {

public:
    Polygon();
    std::vector<Cartesian> getVertices() const;
    void setVertices(const std::vector<Cartesian>& vertices);

private:
    std::vector<Cartesian> m_vertices;
};

} // namespace sim

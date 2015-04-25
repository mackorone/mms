#pragma once

#include "Polygon.h"

namespace sim {

class GraphicUtilities {

public:
    static void drawPolygon(const Polygon& polygon);

private:
    // A private constructor to restrict creation of any GeometryUtilities objects
    GraphicUtilities();

};

} // namespace sim

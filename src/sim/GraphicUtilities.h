#pragma once

#include <glut.h>
#include <string>

#include "Polygon.h"
#include "TriangleGraphic.h"

namespace sim {

class GraphicUtilities {

public:
    static void drawPolygon(const Polygon& polygon, const GLfloat* color, GLfloat alpha);
    static void drawText(const Coordinate& location, const Distance& width, const Distance& height, const std::string& text);

    // TODO
    static std::vector<TriangleGraphic> polygonToTriangleGraphics(const Polygon& polygon, const GLfloat* color, GLfloat alpha);

private:
    // A private constructor to restrict creation of any GeometryUtilities objects
    GraphicUtilities();

    // Returns the width and height (respectively) of the window in pixels
    static std::pair<int, int> getWindowSize();

    // Returns the x and y values (respectively) for openGl coordinates
    static std::pair<float, float> getOpenGlCoordinates(const Coordinate& coordinate);

};

} // namespace sim

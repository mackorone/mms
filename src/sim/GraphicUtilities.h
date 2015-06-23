#pragma once

#include <glut.h>
#include <string>

#include "Polygon.h"
#include "TriangleGraphic.h"

namespace sim {

class GraphicUtilities {

// TODO: consts in here???

public:
    // TODO: This might not actually help things... it might be just as in efficient
    // The big, shared, triangle graphics buffer // TODO: Encapsulate this, perhaps...
    //static std::vector<TriangleGraphic> TGB;
    //static int getMouseStartingIndex();

    // TODO
    static void drawMousePolygon(const Polygon& polygon, const GLfloat* color, GLfloat alpha);
    static void drawTilePolygon(const Polygon& polygon, const GLfloat* color, GLfloat alpha); // TODO

    // TODO: Deprecate these
    static void drawPolygon(const Polygon& polygon, const GLfloat* color, GLfloat alpha);
    static void drawText(const Coordinate& location, const Distance& width, const Distance& height, const std::string& text);

private:
    // TODO
    //static m_mouseStartingIndex;

    // A private constructor to restrict creation of any GeometryUtilities objects
    GraphicUtilities();

    // Returns the width and height (respectively) of the window in pixels
    static std::pair<int, int> getWindowSize();

    // Returns the x and y values (respectively) for openGl coordinates
    static std::pair<float, float> getOpenGlCoordinates(const Coordinate& coordinate);

    // Converts a polygon to a vector of triangle graphics
    static std::vector<TriangleGraphic> polygonToTriangleGraphics(const Polygon& polygon, const GLfloat* color, GLfloat alpha);
};

} // namespace sim

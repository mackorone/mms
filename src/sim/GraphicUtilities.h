#pragma once

#include <glut.h>
#include <string>

#include "Directions.h"
#include "Polygon.h"
#include "TriangleGraphic.h"

namespace sim {

// TODO: Put consts in this class?

class GraphicUtilities {

    // TODO: It might be more expensive to do multiple calls to glBufferData as opposed to one big call...
    // with all of the data from one big CPU-side buffer. I'll have to look into this.

    // TODO: Order in the vector buffer object is important...

    // TODO: "draw" should be understood as "fillTheBufferWithUpdates"
    struct TriangleGraphicUpdate {
        int vboIndex;
        TriangleGraphic triangleGraphic;
    };

public:
    // TODO: Encapsulate this, perhaps...
    static std::vector<TriangleGraphicUpdate> TGUs;

    // TODO: more for text, other tile properties
    static void updateTileGraphicBase(int x, int y, const Polygon& polygon, const GLfloat* color, GLfloat alpha);
    static void updateTileGraphicWall(int x, int y, Direction direction, const Polygon& polygon, const GLfloat* color, GLfloat alpha);
    static void updateTileGraphicCorner(int x, int y, int cornerNumber, const Polygon& polygon, const GLfloat* color, GLfloat alpha);
    static void updateTileGraphicFog(int x, int y, const Polygon& polygon, const GLfloat* color, GLfloat alpha);

    // This is really appendMousePolygonToBuffer TODO
    static void drawMousePolygon(const Polygon& polygon, const GLfloat* color, GLfloat alpha);

    // TODO: Deprecate these
    static void drawText(const Coordinate& location, const Distance& width, const Distance& height, const std::string& text);

private:
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

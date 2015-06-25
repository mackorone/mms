#pragma once

#include <glut.h>
#include <string>

#include "Directions.h"
#include "Polygon.h"
#include "TriangleGraphic.h"

namespace sim {

class GraphicUtilities {

public:

    // The big, CPU-side triangle graphics buffer
    static std::vector<TriangleGraphic> TGB;

    // We need to get this information from the maze graphic
    static void setMazeHeight(int height);

    static void drawTileGraphicBase(int x, int y, const Polygon& polygon, const GLfloat* color);
    static void drawTileGraphicWall(int x, int y, Direction direction, const Polygon& polygon, const GLfloat* color, const GLfloat alpha);
    static void drawTileGraphicCorner(int x, int y, int cornerNumber, const Polygon& polygon, const GLfloat* color);
    static void drawTileGraphicFog(int x, int y, const Polygon& polygon, const GLfloat* color, GLfloat alpha);
    static void drawTileGraphicText(int x, int y, const std::string& text);

    static void updateTileGraphicBaseColor(int x, int y, const GLfloat* color);
    static void updateTileGraphicWallColor(int x, int y, Direction direction, const GLfloat* color, GLfloat alpha);
    static void updateTileGraphicFog(int x, int y, GLfloat alpha);

    static void drawMousePolygon(const Polygon& polygon, const GLfloat* color);

    // TODO: Deprecate this
    static void drawText(const Coordinate& location, const Distance& width, const Distance& height, const std::string& text);

private:

    // A private constructor to restrict creation of any GeometryUtilities objects
    GraphicUtilities();

    // Maze dimensions
    static int s_mazeHeight;

    // Returns the width and height (respectively) of the window in pixels
    static std::pair<int, int> getWindowSize();

    // Returns the x and y values (respectively) for openGl coordinates
    static std::pair<float, float> getOpenGlCoordinates(const Coordinate& coordinate);

    // Converts a polygon to a vector of triangle graphics
    static std::vector<TriangleGraphic> polygonToTriangleGraphics(const Polygon& polygon, const GLfloat* color, GLfloat alpha);

    // Retrieve the indices into the TGB for each specific type of Tile triangle
    static int trianglesPerTile();
    static int getTileGraphicBaseStartingIndex(int x, int y);
    static int getTileGraphicWallStartingIndex(int x, int y, Direction direction);
    static int getTileGraphicCornerStartingIndex(int x, int y, int cornerNumber);
    static int getTileGraphicFogStartingIndex(int x, int y);
};

} // namespace sim

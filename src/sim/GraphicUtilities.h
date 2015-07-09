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
    static void setMazeSize(int mazeWidth, int mazeHeight);

    // Retrieve the 4x4 camera matrices for each map
    static std::vector<float> getFullMapCameraMatrix();
    static std::vector<float> getZoomedMapCameraMatrix(const Coordinate& mouseTranslation, const Angle& mouseRotation);

    // The draw methods here should only be called once - they actually populate the buffer
    static void drawTileGraphicBase(int x, int y, const Polygon& polygon, const GLfloat* color);
    static void drawTileGraphicWall(int x, int y, Direction direction, const Polygon& polygon, const GLfloat* color, const GLfloat alpha);
    static void drawTileGraphicCorner(int x, int y, int cornerNumber, const Polygon& polygon, const GLfloat* color);
    static void drawTileGraphicFog(int x, int y, const Polygon& polygon, const GLfloat* color, GLfloat alpha);
    static void drawTileGraphicText(int x, int y, const std::string& text);

    // These methods are inexpensive, and may be called many times
    static void updateTileGraphicBaseColor(int x, int y, const GLfloat* color);
    static void updateTileGraphicWallColor(int x, int y, Direction direction, const GLfloat* color, GLfloat alpha);
    static void updateTileGraphicFog(int x, int y, GLfloat alpha);

    // TODO: Add an update method for the mouse so that we only triangulate once
    static void drawMousePolygon(const Polygon& polygon, const GLfloat* color, float sensorAlpha);

    // TODO: Deprecate this
    static void drawText(const Coordinate& location, const Distance& width, const Distance& height, const std::string& text);


private:

    // A private constructor to restrict creation of any GeometryUtilities objects
    GraphicUtilities();

    // Maze dimensions
    static int m_mazeWidth;
    static int m_mazeHeight;

    // Returns the x and y values (respectively) for openGl coordinates
    // TODO: Deprecated
    static std::pair<float, float> mapCoordinateToOpenGlCoordinates(const Coordinate& coordinate);
    static float getFractionOfDistance(float start, float end, float location);

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

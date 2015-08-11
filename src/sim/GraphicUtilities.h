#pragma once

#include <glut.h>
#include <string>

#include "Colors.h"
#include "Directions.h"
#include "Polygon.h"
#include "TriangleGraphic.h"

namespace sim {

class GraphicUtilities {

public:

    // The GraphicUtilities class is not constructible.
    GraphicUtilities() = delete;

    // The big, CPU-side triangle graphics buffer
    static std::vector<TriangleGraphic> TGB;

    // Sets the window size, in pixels
    static void setWindowSize(int windowWidth, int windowHeight);

    // Sets the maze size, in tiles
    static void setMazeSize(int mazeWidth, int mazeHeight);

    // Return the positions of the full map and zoomed map, in pixels
    static std::pair<int, int> getFullMapPosition();
    static std::pair<int, int> getZoomedMapPosition();

    // Return the width and height of the full map and zoomed map, in pixels
    static std::pair<int, int> getFullMapSize();
    static std::pair<int, int> getZoomedMapSize();

    // Retrieve the 4x4 transformation matrices for each map
    static std::vector<float> getFullMapTransformationMatrix();
    static std::vector<float> getZoomedMapTransformationMatrix(const Coordinate& initialMouseTranslation,
        const Coordinate& currentMouseTranslation, const Angle& currentMouseRotation);

    // The draw methods here should only be called once - they actually populate the buffer
    static void drawTileGraphicBase(int x, int y, const Polygon& polygon, Color color);
    static void drawTileGraphicWall(int x, int y, Direction direction, const Polygon& polygon, Color color, float alpha);
    static void drawTileGraphicCorner(int x, int y, int cornerNumber, const Polygon& polygon, Color color);
    static void drawTileGraphicDistanceCharacter(int x, int y, int row, int col, const Polygon& polygon, char c); // TODO: MACK
    static void drawTileGraphicFog(int x, int y, const Polygon& polygon, Color color, float alpha);

    // These methods are inexpensive, and may be called many times
    static void updateTileGraphicBaseColor(int x, int y, Color color);
    static void updateTileGraphicWallColor(int x, int y, Direction direction, Color color, float alpha);
    static void updateTileGraphicDistanceCharacter(int x, int y, int row, int col, char c);
    static void updateTileGraphicFog(int x, int y, float alpha);

    // TODO: MACK - Add an update method for the mouse so that we only triangulate once
    static void drawMousePolygon(const Polygon& polygon, Color color, float sensorAlpha);

private:

    // Window dimensions in pixels
    static int m_windowWidth;
    static int m_windowHeight;

    // Maze dimensions in tiles
    static int m_mazeWidth;
    static int m_mazeHeight;

    // Returns the number of pixels per meter of the screen
    static double getScreenPixelsPerMeter();

    // Returns the physical maze size
    static std::pair<double, double> getPhysicalMazeSize();

    // Multiplies two 4x4 matrices together
    static std::vector<float> multiply4x4Matrices(std::vector<float> left, std::vector<float> right);

    // Maps a pixel coordinate ((0,0) in the bottom left, (width, height) in the upper right)
    // to the OpenGL coordinate system ((-1,-1) in the bottom left, (1,1) in the upper right)
    static std::pair<double, double> mapPixelCoordinateToOpenGlCoordinate(double x, double y);

    // Converts a polygon to a vector of triangle graphics
    static std::vector<TriangleGraphic> polygonToTriangleGraphics(const Polygon& polygon, Color color, float alpha);

    // Retrieve the indices into the TGB for each specific type of Tile triangle
    static int trianglesPerTile();
    static int getTileGraphicBaseStartingIndex(int x, int y);
    static int getTileGraphicWallStartingIndex(int x, int y, Direction direction);
    static int getTileGraphicCornerStartingIndex(int x, int y, int cornerNumber);
    static int getTileGraphicDistanceCharacterStartingIndex(int x, int y, int row, int col);
    static int getTileGraphicFogStartingIndex(int x, int y);
};

} // namespace sim

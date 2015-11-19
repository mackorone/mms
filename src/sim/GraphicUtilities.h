#pragma once

#include <string>

#include "Color.h"
#include "Direction.h"
#include "Driver.h"
#include "Polygon.h"
#include "Tile.h"
#include "TriangleGraphic.h"
#include "TriangleTexture.h"

namespace sim {

class GraphicUtilities {

public:

    // The GraphicUtilities class is not constructible.
    GraphicUtilities() = delete;

    // Get and set the window size, in pixels
    static std::pair<int, int> getWindowSize();
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
    static std::vector<float> getZoomedMapTransformationMatrix(
        const Coordinate& initialMouseTranslation, const Angle& initialMouseRotation,
        const Coordinate& currentMouseTranslation, const Angle& currentMouseRotation);

    // Fills the GRAPHIC_CPU_BUFFER and TEXTURE_CPU_BUFFER
    static void insertIntoGraphicCpuBuffer(const Polygon& polygon, Color color, double alpha);
    static void insertIntoTextureCpuBuffer();

    // Returns the maximum number of rows and columns of text in a tile graphic
    static std::pair<int, int> getTileGraphicTextMaxSize();

    // These methods are inexpensive, and may be called many times
    static void updateTileGraphicBaseColor(int x, int y, Color color);
    static void updateTileGraphicWallColor(int x, int y, Direction direction, Color color, double alpha);
    static void updateTileGraphicFog(int x, int y, double alpha);
    static void updateTileGraphicText(const Tile* tile, int numRows, int numCols, int row, int col, char c);

    // TODO: MACK - Add an update method for the mouse so that we only triangulate once
    static void drawMousePolygon(const Polygon& polygon, Color color, double sensorAlpha);

private:

    // CPU-side buffers
    friend sim::Driver;
    static std::vector<TriangleGraphic> GRAPHIC_CPU_BUFFER;
    static std::vector<TriangleTexture> TEXTURE_CPU_BUFFER;

    // Window dimensions in pixels
    static int m_windowWidth;
    static int m_windowHeight;

    // Maze dimensions in tiles
    static int m_mazeWidth;
    static int m_mazeHeight;

    // The characters in the font image
    static const std::string m_fontImageCharacters;

    // Returns the number of pixels per meter of the screen
    static double getScreenPixelsPerMeter();

    // Returns the physical maze size
    static std::pair<double, double> getPhysicalMazeSize();

    // Multiplies two 4x4 matrices together
    static std::vector<float> multiply4x4Matrices(std::vector<float> left, std::vector<float> right);

    // Maps a pixel coordinate ((0,0) in the bottom left, (width, height) in the upper right)
    // to the OpenGL coordinate system ((-1,-1) in the bottom left, (1,1) in the upper right)
    static std::pair<double, double> mapPixelCoordinateToOpenGlCoordinate(double x, double y);

    // Converts a polygon to a vector of triangle graphics or triangle textures
    static std::vector<TriangleGraphic> polygonToTriangleGraphics(const Polygon& polygon, Color color, double alpha);

    // Retrieve the indices into the GRAPHIC_CPU_BUFFER for each specific type of Tile triangle
    static int trianglesPerTile();
    static int getTileGraphicBaseStartingIndex(int x, int y);
    static int getTileGraphicWallStartingIndex(int x, int y, Direction direction);
    static int getTileGraphicCornerStartingIndex(int x, int y, int cornerNumber);
    static int getTileGraphicFogStartingIndex(int x, int y);

    // Retrieve the indices into the TEXTURE_CPU_BUFFER
    static int getTileGraphicTextStartingIndex(int x, int y, int row, int col);

};

} // namespace sim

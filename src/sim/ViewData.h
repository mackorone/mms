#pragma once

#include <string>

#include "Color.h"
#include "Direction.h"
#include "Polygon.h"
#include "Tile.h"
#include "TriangleGraphic.h"
#include "TriangleTexture.h"


namespace sim {

class View; // TODO: MACK

// TODO: MACK - rename this to view handle...
class ViewData {

public:

    ViewData(int mazeWidth, int mazeHeight, View* view);
    View* m_view;

    // Get and set the window size, in pixels
    std::pair<int, int> getWindowSize();

    // Fills the GRAPHIC_CPU_BUFFER and TEXTURE_CPU_BUFFER
    void insertIntoGraphicCpuBuffer(const Polygon& polygon, Color color, double alpha);
    void insertIntoTextureCpuBuffer();

    // Returns the maximum number of rows and columns of text in a tile graphic
    std::pair<int, int> getTileGraphicTextMaxSize(); // TODO

    // These methods are inexpensive, and may be called many times
    void updateTileGraphicBaseColor(int x, int y, Color color);
    void updateTileGraphicWallColor(int x, int y, Direction direction, Color color, double alpha);
    void updateTileGraphicFog(int x, int y, double alpha);
    void updateTileGraphicText(const Tile* tile, int numRows, int numCols, int row, int col, char c);

    // Appends a mouse polygon to the GRAPHIC_CPU_BUFFER
    void drawMousePolygon(const Polygon& polygon, Color color, double sensorAlpha);

    // CPU-side buffers
    std::vector<TriangleGraphic> GRAPHIC_CPU_BUFFER;
    std::vector<TriangleTexture> TEXTURE_CPU_BUFFER;

    // Window dimensions in pixels
    int m_windowWidth;
    int m_windowHeight;

    int m_mazeWidth; // TODO: MACK
    int m_mazeHeight; // TODO: MACK

    // Converts a polygon to a vector of triangle graphics or triangle textures
    std::vector<TriangleGraphic> polygonToTriangleGraphics(const Polygon& polygon, Color color, double alpha);

    // Retrieve the indices into the GRAPHIC_CPU_BUFFER for each specific type of Tile triangle
    int trianglesPerTile();
    int getTileGraphicBaseStartingIndex(int x, int y);
    int getTileGraphicWallStartingIndex(int x, int y, Direction direction);
    int getTileGraphicCornerStartingIndex(int x, int y, int cornerNumber);
    int getTileGraphicFogStartingIndex(int x, int y);

    // Retrieve the indices into the TEXTURE_CPU_BUFFER
    int getTileGraphicTextStartingIndex(int x, int y, int row, int col);

private: // TODO: MACK

};

} // namespace sim

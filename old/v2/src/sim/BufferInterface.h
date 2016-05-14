#pragma once

#include "Color.h"
#include "Direction.h"
#include "Polygon.h"
#include "TileGraphicTextCache.h"
#include "TileTextAlignment.h"
#include "TriangleGraphic.h"
#include "TriangleTexture.h"

namespace sim {

class BufferInterface {

public:
    BufferInterface(
        std::pair<int, int> mazeSize,
        std::vector<TriangleGraphic>* graphicCpuBuffer,
        std::vector<TriangleTexture>* textureCpuBuffer);

    // Initializes and caches all possible tile text positions. We need this
    // extra initialization function since the max size is from the algorithm.
    void initTileGraphicText(
        const Distance& wallLength,
        const Distance& wallWidth,
        std::pair<int, int> tileGraphicTextMaxSize,
        const std::map<char, std::pair<double, double>>& fontImageMap,
        double borderFraction,
        TileTextAlignment tileTextAlignment);

    // Returns the maximum number of rows and columns of text in a tile graphic
    std::pair<int, int> getTileGraphicTextMaxSize();

    // Fills the graphic cpu buffer and texture cpu buffer
    void insertIntoGraphicCpuBuffer(const Polygon& polygon, Color color, double alpha);
    void insertIntoTextureCpuBuffer();

    // These methods are inexpensive, and may be called many times
    void updateTileGraphicBaseColor(int x, int y, Color color);
    void updateTileGraphicWallColor(int x, int y, Direction direction, Color color, double alpha);
    void updateTileGraphicFog(int x, int y, double alpha);
    void updateTileGraphicText(int x, int y, int numRows, int numCols, int row, int col, char c);

    // Appends a mouse polygon to the graphic cpu buffer
    void drawMousePolygon(const Polygon& polygon, Color color, double sensorAlpha);

private:

    // The width and height of the maze
    std::pair<int, int> m_mazeSize;

    // CPU-side buffers
    std::vector<TriangleGraphic>* m_graphicCpuBuffer;
    std::vector<TriangleTexture>* m_textureCpuBuffer;

    // A cache for tile graphic text information
    TileGraphicTextCache m_tileGraphicTextCache;

    // Converts a polygon to a vector of triangle graphics or triangle textures
    std::vector<TriangleGraphic> polygonToTriangleGraphics(const Polygon& polygon, Color color, double alpha);

    // Retrieve the indices into the graphic cpu buffer for each specific type of Tile triangle
    int trianglesPerTile();
    int getTileGraphicBaseStartingIndex(int x, int y);
    int getTileGraphicWallStartingIndex(int x, int y, Direction direction);
    int getTileGraphicCornerStartingIndex(int x, int y, int cornerNumber);
    int getTileGraphicFogStartingIndex(int x, int y);

    // Retrieve the indices into the texture cpu buffer
    int getTileGraphicTextStartingIndex(int x, int y, int row, int col);

};

} // namespace sim

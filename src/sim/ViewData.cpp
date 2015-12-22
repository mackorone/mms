#include "ViewData.h"

#include <algorithm>

#include <glut/glut.h>

#include "Assert.h"
#include "CPMath.h"
#include "GeometryUtilities.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"
#include "Triangle.h"
#include "VertexTexture.h"

#include "View.h" // TODO: MACK

namespace sim {

ViewData::ViewData(int mazeWidth, int mazeHeight, View* view) : m_view(view) {
    // TODO: MACK - kill these
    m_mazeWidth = mazeWidth;
    m_mazeHeight = mazeHeight;
}

// TODO: MACK - do we need this?
std::pair<int, int> ViewData::getWindowSize() {
    return m_view->getWindowSize();
}

// These values must perfectly reflect the font image being used
// TODO: MACK - make this into a map
const std::string FONT_IMAGE_CHARS =
    " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

void ViewData::insertIntoGraphicCpuBuffer(const Polygon& polygon, Color color, double alpha) {
    std::vector<TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, alpha);
    for (int i = 0; i < tgs.size(); i += 1) {
        GRAPHIC_CPU_BUFFER.push_back(tgs.at(i));
    }
}

void ViewData::insertIntoTextureCpuBuffer() {
    // Here we just insert dummy TriangleTexture objects. All of the actual
    // values of the objects will be set on calls to the update method.
    TriangleTexture t {
        {0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0},
    };
    // There are exactly two triangle texture objects needed, since there are
    // two triangles per rectangular texture
    for (int i = 0; i < 2; i += 1) {
        TEXTURE_CPU_BUFFER.push_back(t);
    }
}

std::pair<int, int> ViewData::getTileGraphicTextMaxSize() {
    static std::pair<int, int> maxRowsAndCols = std::make_pair(2, 4);
    return maxRowsAndCols;
}

void ViewData::updateTileGraphicBaseColor(int x, int y, Color color) {
    int index = getTileGraphicBaseStartingIndex(x, y);
    std::tuple<double, double, double> colorValues = COLOR_TO_RGB.at(color);
    for (int i = 0; i < 2; i += 1) {
        TriangleGraphic* triangleGraphic = &GRAPHIC_CPU_BUFFER.at(index + i);
        triangleGraphic->p1.r = std::get<0>(colorValues);
        triangleGraphic->p1.g = std::get<1>(colorValues);
        triangleGraphic->p1.b = std::get<2>(colorValues);
        triangleGraphic->p2.r = std::get<0>(colorValues);
        triangleGraphic->p2.g = std::get<1>(colorValues);
        triangleGraphic->p2.b = std::get<2>(colorValues);
        triangleGraphic->p3.r = std::get<0>(colorValues);
        triangleGraphic->p3.g = std::get<1>(colorValues);
        triangleGraphic->p3.b = std::get<2>(colorValues);
    }
}

void ViewData::updateTileGraphicWallColor(int x, int y, Direction direction, Color color, double alpha) {
    int index = getTileGraphicWallStartingIndex(x, y, direction);
    std::tuple<double, double, double> colorValues = COLOR_TO_RGB.at(color);
    for (int i = 0; i < 2; i += 1) {
        TriangleGraphic* triangleGraphic = &GRAPHIC_CPU_BUFFER.at(index + i);
        triangleGraphic->p1.r = std::get<0>(colorValues);
        triangleGraphic->p1.g = std::get<1>(colorValues);
        triangleGraphic->p1.b = std::get<2>(colorValues);
        triangleGraphic->p1.a = alpha;
        triangleGraphic->p2.r = std::get<0>(colorValues);
        triangleGraphic->p2.g = std::get<1>(colorValues);
        triangleGraphic->p2.b = std::get<2>(colorValues);
        triangleGraphic->p2.a = alpha;
        triangleGraphic->p3.r = std::get<0>(colorValues);
        triangleGraphic->p3.g = std::get<1>(colorValues);
        triangleGraphic->p3.b = std::get<2>(colorValues);
        triangleGraphic->p3.a = alpha;
    }
}

void ViewData::updateTileGraphicFog(int x, int y, double alpha) {
    int index = getTileGraphicFogStartingIndex(x, y);
    for (int i = 0; i < 2; i += 1) {
        TriangleGraphic* triangleGraphic = &GRAPHIC_CPU_BUFFER.at(index + i);
        triangleGraphic->p1.a = alpha;
        triangleGraphic->p2.a = alpha;
        triangleGraphic->p3.a = alpha;
    }
}

void ViewData::updateTileGraphicText(const Tile* tile, int numRows, int numCols, int row, int col, char c) {

    //                          col
    //
    //             *---*-------------------*---*
    //             |   |                   |   |
    //             *---*------------------[Y]--*
    //             |   |    |    |    |    |   |
    //             |   |    |    |    |    |   |
    //             |   | 00 | 01 | 02 | 03 |   |
    // rowsFromTop |   |____|____|____|____|   |
    //             |   |    |    |    |    |   |
    //             |   |    |    |    |    |   |
    //             |   | 10 | 11 | 12 | 13 |   |
    //             |   |    |    |    |    |   |
    //             *--[X]------------------*---*
    //             |   |                   |   |
    //             *---*-------------------*---*

    std::pair<int, int> maxRowsAndCols = ViewData::getTileGraphicTextMaxSize();
    Cartesian X = tile->getInteriorPolygon().getVertices().at(0);
    Cartesian Y = tile->getInteriorPolygon().getVertices().at(2);
    Cartesian diagonal = Y - X;
    Meters characterWidth = diagonal.getX() / static_cast<double>(maxRowsAndCols.second);
    Meters characterHeight = diagonal.getY() / static_cast<double>(maxRowsAndCols.first);

    // The rows start from the top and increase down the cell
    int rowsFromTop = maxRowsAndCols.first - row - 1;
    double rowOffset = rowsFromTop - static_cast<double>(maxRowsAndCols.first - numRows) / 2.0;
    double colOffset = col + static_cast<double>(maxRowsAndCols.second - numCols) / 2.0;

    int fontImageCharacterIndex = FONT_IMAGE_CHARS.find_first_of(c);
    VertexTexture p1 = { // LL
        (X + Cartesian(characterWidth *  colOffset     , characterHeight *  rowOffset     )).getX().getMeters(),
        (X + Cartesian(characterWidth *  colOffset     , characterHeight *  rowOffset     )).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex) / static_cast<double>(FONT_IMAGE_CHARS.size()),
        0.0,
    };
    VertexTexture p2 = { // UL
        (X + Cartesian(characterWidth *  colOffset     , characterHeight * (rowOffset + 1))).getX().getMeters(),
        (X + Cartesian(characterWidth *  colOffset     , characterHeight * (rowOffset + 1))).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex) / static_cast<double>(FONT_IMAGE_CHARS.size()),
        1.0,
    };
    VertexTexture p3 = { // UR
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight * (rowOffset + 1))).getX().getMeters(),
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight * (rowOffset + 1))).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex + 1) / static_cast<double>(FONT_IMAGE_CHARS.size()),
        1.0,
    };
    VertexTexture p4 = { // LR
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight *  rowOffset     )).getX().getMeters(),
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight *  rowOffset     )).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex + 1) / static_cast<double>(FONT_IMAGE_CHARS.size()),
        0.0,
    };

    int triangleTextureIndex = getTileGraphicTextStartingIndex(tile->getX(), tile->getY(), row, col);
    TEXTURE_CPU_BUFFER.at(triangleTextureIndex) = {p1, p2, p3};
    TEXTURE_CPU_BUFFER.at(triangleTextureIndex + 1) = {p1, p3, p4};
}

void ViewData::drawMousePolygon(const Polygon& polygon, Color color, double sensorAlpha) {
    std::vector<TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, sensorAlpha);
    GRAPHIC_CPU_BUFFER.insert(GRAPHIC_CPU_BUFFER.end(), tgs.begin(), tgs.end());
}

std::vector<TriangleGraphic> ViewData::polygonToTriangleGraphics(const Polygon& polygon, Color color, double alpha) {
    std::vector<Triangle> triangles = polygon.getTriangles();
    std::vector<TriangleGraphic> triangleGraphics;
    std::tuple<double, double, double> colorValues = COLOR_TO_RGB.at(color);
    for (Triangle triangle : triangles) {
        triangleGraphics.push_back({
            {triangle.p1.getX().getMeters(), triangle.p1.getY().getMeters(),
                std::get<0>(colorValues), std::get<1>(colorValues), std::get<2>(colorValues), alpha},
            {triangle.p2.getX().getMeters(), triangle.p2.getY().getMeters(),
                std::get<0>(colorValues), std::get<1>(colorValues), std::get<2>(colorValues), alpha},
            {triangle.p3.getX().getMeters(), triangle.p3.getY().getMeters(),
                std::get<0>(colorValues), std::get<1>(colorValues), std::get<2>(colorValues), alpha}});
    }
    return triangleGraphics;
}

int ViewData::trianglesPerTile() {
    // This value must be predetermined, and was done so as follows:
    // Base polygon:      2 (2 triangles x 1 polygon  per tile)
    // Wall polygon:      8 (2 triangles x 4 polygons per tile)
    // Corner polygon:    8 (2 triangles x 4 polygons per tile)
    // Fog polygon:       2 (2 triangles x 1 polygon  per tile)
    // --------------------
    // Total             20
    return 20;
}

int ViewData::getTileGraphicBaseStartingIndex(int x, int y) {
    return  0 + trianglesPerTile() * (m_mazeHeight * x + y);
}

int ViewData::getTileGraphicWallStartingIndex(int x, int y, Direction direction) {
    return  2 + trianglesPerTile() * (m_mazeHeight * x + y) + (2 * SimUtilities::getDirectionIndex(direction));
}

int ViewData::getTileGraphicCornerStartingIndex(int x, int y, int cornerNumber) {
    return 10 + trianglesPerTile() * (m_mazeHeight * x + y) + (2 * cornerNumber);
}

int ViewData::getTileGraphicFogStartingIndex(int x, int y) {
    return 18 + trianglesPerTile() * (m_mazeHeight * x + y);
}

int ViewData::getTileGraphicTextStartingIndex(int x, int y, int row, int col) {
    static std::pair<int, int> maxRowsAndCols = getTileGraphicTextMaxSize();
    static int triangleTexturesPerTile = 2 * maxRowsAndCols.first * maxRowsAndCols.second;
    return triangleTexturesPerTile * (m_mazeHeight * x + y) + 2 * (row * maxRowsAndCols.second + col);
}

} // namespace sim

#include "BufferInterface.h"

#include "Assert.h"
#include "ContainerUtilities.h"
#include "Logging.h"
#include "SimUtilities.h"

namespace sim {

BufferInterface::BufferInterface(
        std::pair<int, int> mazeSize,
        std::pair<int, int> tileGraphicTextMaxSize,
        std::map<char, int> fontImageMap,
        std::vector<TriangleGraphic>* graphicCpuBuffer,
        std::vector<TriangleTexture>* textureCpuBuffer) :
        m_mazeSize(mazeSize),
        m_tileGraphicTextMaxSize(tileGraphicTextMaxSize),
        m_fontImageMap(fontImageMap),
        m_graphicCpuBuffer(graphicCpuBuffer),
        m_textureCpuBuffer(textureCpuBuffer) {
}

std::pair<int, int> BufferInterface::getTileGraphicTextMaxSize() {
    return m_tileGraphicTextMaxSize;
}

void BufferInterface::insertIntoGraphicCpuBuffer(const Polygon& polygon, Color color, double alpha) {
    std::vector<TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, alpha);
    for (int i = 0; i < tgs.size(); i += 1) {
        m_graphicCpuBuffer->push_back(tgs.at(i));
    }
}

void BufferInterface::insertIntoTextureCpuBuffer() {
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
        m_textureCpuBuffer->push_back(t);
    }
}

void BufferInterface::updateTileGraphicBaseColor(int x, int y, Color color) {
    int index = getTileGraphicBaseStartingIndex(x, y);
    std::tuple<double, double, double> colorValues = COLOR_TO_RGB.at(color);
    for (int i = 0; i < 2; i += 1) {
        TriangleGraphic* triangleGraphic = &m_graphicCpuBuffer->at(index + i);
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

void BufferInterface::updateTileGraphicWallColor(int x, int y, Direction direction, Color color, double alpha) {
    int index = getTileGraphicWallStartingIndex(x, y, direction);
    std::tuple<double, double, double> colorValues = COLOR_TO_RGB.at(color);
    for (int i = 0; i < 2; i += 1) {
        TriangleGraphic* triangleGraphic = &m_graphicCpuBuffer->at(index + i);
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

void BufferInterface::updateTileGraphicFog(int x, int y, double alpha) {
    int index = getTileGraphicFogStartingIndex(x, y);
    for (int i = 0; i < 2; i += 1) {
        TriangleGraphic* triangleGraphic = &m_graphicCpuBuffer->at(index + i);
        triangleGraphic->p1.a = alpha;
        triangleGraphic->p2.a = alpha;
        triangleGraphic->p3.a = alpha;
    }
}

void BufferInterface::updateTileGraphicText(const Tile* tile, int numRows, int numCols, int row, int col, char c) {

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

    std::pair<int, int> maxRowsAndCols = BufferInterface::getTileGraphicTextMaxSize();
    Cartesian X = tile->getInteriorPolygon().getVertices().at(0);
    Cartesian Y = tile->getInteriorPolygon().getVertices().at(2);
    Cartesian diagonal = Y - X;
    Meters characterWidth = diagonal.getX() / static_cast<double>(maxRowsAndCols.second);
    Meters characterHeight = diagonal.getY() / static_cast<double>(maxRowsAndCols.first);

    // The rows start from the top and increase down the cell
    int rowsFromTop = maxRowsAndCols.first - row - 1;
    double rowOffset = rowsFromTop - static_cast<double>(maxRowsAndCols.first - numRows) / 2.0;
    double colOffset = col + static_cast<double>(maxRowsAndCols.second - numCols) / 2.0;

    ASSERT_TR(ContainerUtilities::mapContains(m_fontImageMap, c));
    int fontImageCharacterIndex = m_fontImageMap.at(c);

    VertexTexture p1 = { // LL
        (X + Cartesian(characterWidth *  colOffset     , characterHeight *  rowOffset     )).getX().getMeters(),
        (X + Cartesian(characterWidth *  colOffset     , characterHeight *  rowOffset     )).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex) / static_cast<double>(m_fontImageMap.size()),
        0.0,
    };
    VertexTexture p2 = { // UL
        (X + Cartesian(characterWidth *  colOffset     , characterHeight * (rowOffset + 1))).getX().getMeters(),
        (X + Cartesian(characterWidth *  colOffset     , characterHeight * (rowOffset + 1))).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex) / static_cast<double>(m_fontImageMap.size()),
        1.0,
    };
    VertexTexture p3 = { // UR
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight * (rowOffset + 1))).getX().getMeters(),
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight * (rowOffset + 1))).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex + 1) / static_cast<double>(m_fontImageMap.size()),
        1.0,
    };
    VertexTexture p4 = { // LR
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight *  rowOffset     )).getX().getMeters(),
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight *  rowOffset     )).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex + 1) / static_cast<double>(m_fontImageMap.size()),
        0.0,
    };

    int triangleTextureIndex = getTileGraphicTextStartingIndex(tile->getX(), tile->getY(), row, col);
    m_textureCpuBuffer->at(triangleTextureIndex) = {p1, p2, p3};
    m_textureCpuBuffer->at(triangleTextureIndex + 1) = {p1, p3, p4};
}

void BufferInterface::drawMousePolygon(const Polygon& polygon, Color color, double sensorAlpha) {
    std::vector<TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, sensorAlpha);
    m_graphicCpuBuffer->insert(m_graphicCpuBuffer->end(), tgs.begin(), tgs.end());
}

std::vector<TriangleGraphic> BufferInterface::polygonToTriangleGraphics(const Polygon& polygon, Color color, double alpha) {
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

int BufferInterface::trianglesPerTile() {
    // This value must be predetermined, and was done so as follows:
    // Base polygon:      2 (2 triangles x 1 polygon  per tile)
    // Wall polygon:      8 (2 triangles x 4 polygons per tile)
    // Corner polygon:    8 (2 triangles x 4 polygons per tile)
    // Fog polygon:       2 (2 triangles x 1 polygon  per tile)
    // --------------------
    // Total             20
    return 20;
}

int BufferInterface::getTileGraphicBaseStartingIndex(int x, int y) {
    return  0 + trianglesPerTile() * (m_mazeSize.second * x + y);
}

int BufferInterface::getTileGraphicWallStartingIndex(int x, int y, Direction direction) {
    return  2 + trianglesPerTile() * (m_mazeSize.second * x + y) + (2 * SimUtilities::getDirectionIndex(direction));
}

int BufferInterface::getTileGraphicCornerStartingIndex(int x, int y, int cornerNumber) {
    return 10 + trianglesPerTile() * (m_mazeSize.second * x + y) + (2 * cornerNumber);
}

int BufferInterface::getTileGraphicFogStartingIndex(int x, int y) {
    return 18 + trianglesPerTile() * (m_mazeSize.second * x + y);
}

int BufferInterface::getTileGraphicTextStartingIndex(int x, int y, int row, int col) {
    static std::pair<int, int> maxRowsAndCols = getTileGraphicTextMaxSize();
    static int triangleTexturesPerTile = 2 * maxRowsAndCols.first * maxRowsAndCols.second;
    return triangleTexturesPerTile * (m_mazeSize.second * x + y) + 2 * (row * maxRowsAndCols.second + col);
}

} // namespace sim

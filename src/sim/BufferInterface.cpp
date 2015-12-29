#include "BufferInterface.h"

#include "Assert.h"
#include "ContainerUtilities.h"
#include "Logging.h"
#include "SimUtilities.h"

namespace sim {

BufferInterface::BufferInterface(
        std::pair<int, int> mazeSize,
        std::vector<TriangleGraphic>* graphicCpuBuffer,
        std::vector<TriangleTexture>* textureCpuBuffer) :
        m_mazeSize(mazeSize),
        m_graphicCpuBuffer(graphicCpuBuffer),
        m_textureCpuBuffer(textureCpuBuffer) {
}

void BufferInterface::initTileGraphicText(
        const Distance& wallLength,
        const Distance& wallWidth,
        std::pair<int, int> tileGraphicTextMaxSize,
        const std::map<char, std::pair<double, double>>& fontImageMap,
        double borderFraction,
        TileTextAlignment tileTextAlignment) {

    m_tileGraphicTextCache.init(
        wallLength,
        wallWidth,
        tileGraphicTextMaxSize,
        fontImageMap,
        borderFraction,
        tileTextAlignment
    );
}

std::pair<int, int> BufferInterface::getTileGraphicTextMaxSize() {
    return m_tileGraphicTextCache.getTileGraphicTextMaxSize();
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
    // However, we do intentionally insert the appropriate 'v' values, since
    // these will never change.
    TriangleTexture t1 {
        // x    y    u    v
        {0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 1.0},
        {0.0, 0.0, 0.0, 1.0},
    };
    TriangleTexture t2 {
        {0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 1.0},
        {0.0, 0.0, 0.0, 0.0},
    };
    m_textureCpuBuffer->push_back(t1);
    m_textureCpuBuffer->push_back(t2);
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

void BufferInterface::updateTileGraphicText(int x, int y, int numRows, int numCols, int row, int col, char c) {

    //    +---------[UR]  [p2]-------[p3]    [p2]
    //    |         / |    |         /       / |
    //    |  t1   /   |    |  t1   /       /   |
    //    |     /     |    |     /       /     |
    //    |   /   t2  |    |   /       /   t2  |
    //    | /         |    | /       /         |
    //   [LL]---------+   [p1]     [p1]------[p3]

    std::pair<double, double> fontImageCharacterPosition =
        m_tileGraphicTextCache.getFontImageCharacterPosition(c);

    std::pair<Cartesian, Cartesian> LL_UR =
        m_tileGraphicTextCache.getTileGraphicTextPosition(x, y, numRows, numCols, row, col);

    int triangleTextureIndex = getTileGraphicTextStartingIndex(x, y, row, col);
    TriangleTexture* t1 = &m_textureCpuBuffer->at(triangleTextureIndex);
    TriangleTexture* t2 = &m_textureCpuBuffer->at(triangleTextureIndex + 1);

    t1->p1.x = LL_UR.first.getX().getMeters();
    t1->p1.y = LL_UR.first.getY().getMeters();
    t1->p1.u = fontImageCharacterPosition.first;
    t1->p2.x = LL_UR.first.getX().getMeters();
    t1->p2.y = LL_UR.second.getY().getMeters();
    t1->p2.u = fontImageCharacterPosition.first;
    t1->p3.x = LL_UR.second.getX().getMeters();
    t1->p3.y = LL_UR.second.getY().getMeters();
    t1->p3.u = fontImageCharacterPosition.second;

    t2->p1.x = LL_UR.first.getX().getMeters();
    t2->p1.y = LL_UR.first.getY().getMeters();
    t2->p1.u = fontImageCharacterPosition.first;
    t2->p2.x = LL_UR.second.getX().getMeters();
    t2->p2.y = LL_UR.second.getY().getMeters();
    t2->p2.u = fontImageCharacterPosition.second;
    t2->p3.x = LL_UR.second.getX().getMeters();
    t2->p3.y = LL_UR.first.getY().getMeters();
    t2->p3.u = fontImageCharacterPosition.second;
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

#include "BufferInterface.h"

#include "Assert.h"
#include "ContainerUtilities.h"
#include "Logging.h"
#include "SimUtilities.h"

namespace sim {

BufferInterface::BufferInterface(
        std::pair<int, int> mazeSize,
        const Meters& wallLength,
        const Meters& wallWidth,
        std::vector<TriangleGraphic>* graphicCpuBuffer,
        std::vector<TriangleTexture>* textureCpuBuffer) :
        m_mazeSize(mazeSize),
        m_wallLength(wallLength),
        m_wallWidth(wallWidth),
        m_graphicCpuBuffer(graphicCpuBuffer),
        m_textureCpuBuffer(textureCpuBuffer) {
}

void BufferInterface::initTileGraphicText(
        std::pair<int, int> tileGraphicTextMaxSize,
        const std::map<char, int>& fontImageMap,
        double borderFraction,
        TileTextAlignment tileTextAlignment) {

    // First we assign the text max size and font image map
    m_tileGraphicTextMaxSize = tileGraphicTextMaxSize;
    m_fontImageMap = fontImageMap;

    // Build a cache for the tile text updates
    // 
    //                    col
    //     *-*---------------------------*-*    *-*---------------------------*-*
    //     *-*--------------------------[B]*    *-*--------------------------[B]*
    //     | |                           | |    | |                           | |
    //     | |   *------------------[D]  | |    | |   *------------------[D]  | |
    //     | |   |    |    |    |    |   | |    | |   |                   |   | |
    //     | |   |    |    |    |    |   | |    | |   |                   |   | |
    //     | |   |    | 00 | 01 |    |   | |    | |   |----|----|----|----|   | |
    // row | |   |    |____|____|    |   | | or | |   |    |    |    |    |   | |
    //     | |   |    |    |    |    |   | |    | |   | 00 | 01 | 02 | 03 |   | |
    //     | |   |    |    |    |    |   | |    | |  [E]---|----|----|----|   | |
    //     | |   |    | 10 | 11 |    |   | |    | |   |                   |   | |
    //     | |   |    |    |    |    |   | |    | |   |                   |   | |
    //     | |  [C]--[E]-------------*   | |    | |  [C]------------------*   | |
    //     | |                           | |    | |                           | |
    //     *[A]--------------------------*-*    *[A]--------------------------*-*
    //     *-*---------------------------*-*    *-*---------------------------*-*

    int maxRows = tileGraphicTextMaxSize.first;
    int maxCols = tileGraphicTextMaxSize.second;

    // First we get the unscaled diagonal
    Cartesian A = Cartesian(m_wallWidth / 2.0, m_wallWidth / 2.0);
    Cartesian B = A + Cartesian(m_wallLength, m_wallLength);
    Cartesian C = A + Cartesian(m_wallLength, m_wallLength) * borderFraction;
    Cartesian D = B - Cartesian(m_wallLength, m_wallLength) * borderFraction;
    Cartesian CD = D - C;

    // We assume that each character is twice as tall as it is wide, and we scale accordingly
    Meters characterWidth = CD.getX() / static_cast<double>(maxCols);
    Meters characterHeight = CD.getY() / static_cast<double>(maxRows);
    if (characterWidth * 2.0 < characterHeight) {
        characterHeight = characterWidth * 2.0;
    }
    else {
        characterWidth = characterHeight / 2.0;
    }

    // Now we get the scaled diagonal (note that we'll only shrink in at most one direction)
    Cartesian scalingOffset = Cartesian(
        (CD.getX() - characterWidth * maxCols) / 2.0,
        (CD.getY() - characterHeight * maxRows) / 2.0
    );
    Cartesian E = C + scalingOffset;

    // For all numbers of rows displayed
    for (int numRows = 0; numRows <= maxRows; numRows += 1) {

        // For all numbers of columns displayed
        for (int numCols = 0; numCols <= maxCols; numCols += 1) {

            // For each visible row and col
            for (int row = 0; row <= maxRows; row += 1) {
                for (int col = 0; col <= maxCols; col += 1) {

                    Cartesian LL = Cartesian(Meters(0), Meters(0));
                    Cartesian UR = Cartesian(Meters(0), Meters(0));

                    if (row < numRows && col < numCols) {

                        double rowOffset = 0.0;
                        if (ContainerUtilities::setContains(CENTER_STAR_ALIGNMENTS, tileTextAlignment)) {
                            rowOffset = static_cast<double>(maxRows - numRows) / 2.0;
                        }
                        else if (ContainerUtilities::setContains(UPPER_STAR_ALIGNMENTS, tileTextAlignment)) {
                            rowOffset = static_cast<double>(maxRows - numRows);
                        }

                        double colOffset = 0.0;
                        if (ContainerUtilities::setContains(STAR_CENTER_ALIGNMENTS, tileTextAlignment)) {
                            colOffset = static_cast<double>(maxCols - numCols) / 2.0;
                        }
                        else if (ContainerUtilities::setContains(STAR_RIGHT_ALIGNMENTS, tileTextAlignment)) {
                            colOffset = static_cast<double>(maxCols - numCols);
                        }

                        LL = Cartesian(
                            Meters(E.getX() + characterWidth * (col + colOffset)),
                            Meters(E.getY() + characterHeight * ((numRows - row - 1) + rowOffset))
                        );
                        UR = Cartesian(
                            Meters(E.getX() + characterWidth * (col + colOffset + 1)),
                            Meters(E.getY() + characterHeight * ((numRows - row - 1) + rowOffset + 1))
                        );
                    }

                    m_tileGraphicTextPositions.insert(
                        std::make_pair(
                            std::make_pair(
                                // The number of rows/cols to be drawn
                                std::make_pair(numRows, numCols),
                                // The row and col of the current character
                                std::make_pair(row, col)
                            ),
                            // The lower left and upper right texture coordinate
                            std::make_pair(LL, UR)
                        )
                    );
                }
            }
        }
    }
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

void BufferInterface::updateTileGraphicText(int x, int y, int numRows, int numCols, int row, int col, char c) {

    // First, get the character index in the font image
    ASSERT_TR(ContainerUtilities::mapContains(m_fontImageMap, c));
    int fontImageCharacterIndex = m_fontImageMap.at(c);

    // Get the character position in the maze for the starting tile
    std::pair<Cartesian, Cartesian> textPosition = m_tileGraphicTextPositions.at(
        std::make_pair(
            std::make_pair(numRows, numCols),
            std::make_pair(row, col)
        )
    );

    // Now get the character position in the maze for *this* tile
    Meters tileLength = m_wallLength + m_wallWidth;
    Cartesian tileOffset = Cartesian(tileLength * x, tileLength * y);
    Cartesian LL = textPosition.first + tileOffset;
    Cartesian UR = textPosition.second + tileOffset;
    Cartesian UL = Cartesian(LL.getX(), UR.getY());
    Cartesian LR = Cartesian(UR.getX(), LL.getY());

    VertexTexture p1 = { // LL
        LL.getX().getMeters(),
        LL.getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex) / static_cast<double>(m_fontImageMap.size()),
        0.0,
    };
    VertexTexture p2 = { // UL
        UL.getX().getMeters(),
        UL.getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex) / static_cast<double>(m_fontImageMap.size()),
        1.0,
    };
    VertexTexture p3 = { // UR
        UR.getX().getMeters(),
        UR.getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex + 1) / static_cast<double>(m_fontImageMap.size()),
        1.0,
    };
    VertexTexture p4 = { // LR
        LR.getX().getMeters(),
        LR.getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex + 1) / static_cast<double>(m_fontImageMap.size()),
        0.0,
    };

    int triangleTextureIndex = getTileGraphicTextStartingIndex(x, y, row, col);
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

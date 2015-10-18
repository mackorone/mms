#include "GraphicUtilities.h"

#include <algorithm>

#include <glut/glut.h>

#include "Assert.h"
#include "CPMath.h"
#include "CPMinMax.h"
#include "GeometryUtilities.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"
#include "Triangle.h"
#include "VertexTexture.h"

namespace sim {

std::vector<TriangleGraphic> GraphicUtilities::GRAPHIC_CPU_BUFFER;
std::vector<TriangleTexture> GraphicUtilities::TEXTURE_CPU_BUFFER;

int GraphicUtilities::m_windowWidth = 0;
int GraphicUtilities::m_windowHeight = 0;

int GraphicUtilities::m_mazeWidth = 0;
int GraphicUtilities::m_mazeHeight = 0;

// These values must perfectly reflect the font image being used
const std::string GraphicUtilities::m_fontImageCharacters = "0123456789 inf";

std::pair<int, int> GraphicUtilities::getWindowSize() {
    return std::make_pair(m_windowWidth, m_windowHeight);
}

void GraphicUtilities::setWindowSize(int windowWidth, int windowHeight) {
    ASSERT_LESS_THAN(0, windowWidth);
    ASSERT_LESS_THAN(0, windowHeight);
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
}

void GraphicUtilities::setMazeSize(int mazeWidth, int mazeHeight) {
    ASSERT_EQUAL(m_mazeWidth, 0);
    ASSERT_EQUAL(m_mazeHeight, 0);
    ASSERT_LESS_THAN(0, mazeWidth);
    ASSERT_LESS_THAN(0, mazeHeight);
    m_mazeWidth = mazeWidth;
    m_mazeHeight = mazeHeight;
}

std::pair<int, int> GraphicUtilities::getFullMapPosition() {
    return std::make_pair(P()->windowBorderWidth(), P()->windowBorderWidth());
}

std::pair<int, int> GraphicUtilities::getZoomedMapPosition() {
    if (S()->layout() == Layout::BOTH) {
        return std::make_pair((m_windowWidth + P()->windowBorderWidth()) / 2, P()->windowBorderWidth());
    }
    return std::make_pair(P()->windowBorderWidth(), P()->windowBorderWidth());
}

std::pair<int, int> GraphicUtilities::getFullMapSize() {
    int width = m_windowWidth - 2 * P()->windowBorderWidth();
    int height = m_windowHeight - 2 * P()->windowBorderWidth();// - 100; // TODO: MACK - add an info panel
    if (S()->layout() == Layout::ZOOMED) {
        width = 0;
    }
    else if (S()->layout() == Layout::BOTH) {
        width = (width - P()->windowBorderWidth()) / 2;
    }
    return std::make_pair(width, height);
}

std::pair<int, int> GraphicUtilities::getZoomedMapSize() {
    int width = m_windowWidth - 2 * P()->windowBorderWidth();
    int height = m_windowHeight - 2 * P()->windowBorderWidth();// - 100; // TODO: MACK - add an info panel
    if (S()->layout() == Layout::FULL) {
        width = 0;
    }
    else if (S()->layout() == Layout::BOTH) {
        width = (width - P()->windowBorderWidth()) / 2;
    }
    return std::make_pair(width, height);
}

std::vector<float> GraphicUtilities::getFullMapTransformationMatrix() {

    // The purpose of this function is to produce a 4x4 matrix which, when
    // applied to the physical coordinate within the vertex shader, transforms
    // it into an OpenGL coordinate for the full map. In this case, the zoomed
    // map always contains the entirety of the maze.

    // Step 1: The physical point (0,0) corresponds to the middle of the
    // bottom-left corner piece:
    //                                 |
    //                                 |--------
    //                                 |       |
    //                                 |   X   |
    //                                 |       |
    //                                 |------------
    //
    // However, we want to make sure that the entire maze is visible within the
    // map window. To ensure this, we first have to translate the physical
    // positions so that (0,0) actually refers to the bottom-left corner of the
    // bottom-left corner:
    //
    //                                 |
    //                                 |--------
    //                                 |       |
    //                                 |       |
    //                                 |       |
    //                                 X------------
    //
    std::vector<float> initialTranslationMatrix = {
        1.0, 0.0, 0.0, static_cast<float>(0.5 * P()->wallWidth()),
        0.0, 1.0, 0.0, static_cast<float>(0.5 * P()->wallWidth()),
        0.0, 0.0, 1.0,                                        0.0,
        0.0, 0.0, 0.0,                                        1.0,
    };

    // Ensure that the maze width and height always appear equally scaled.
    std::pair<double, double> physicalMazeSize = getPhysicalMazeSize();
    double physicalWidth = physicalMazeSize.first;
    double physicalHeight = physicalMazeSize.second;

    // Note that this is not literally the number of pixels per meter of the
    // screen. Rather, it's our desired number of pixels per simulation meter.
    std::pair<int, int> fullMapSize = getFullMapSize();
    double pixelsPerMeter = std::min(fullMapSize.first / physicalWidth, fullMapSize.second / physicalHeight);
    double pixelWidth = pixelsPerMeter * physicalWidth;
    double pixelHeight = pixelsPerMeter * physicalHeight;

    std::pair<double, double> openGlOrigin = mapPixelCoordinateToOpenGlCoordinate(0, 0);
    std::pair<double, double> openGlMazeSize = mapPixelCoordinateToOpenGlCoordinate(pixelWidth, pixelHeight);
    double openGlWidth = openGlMazeSize.first - openGlOrigin.first;
    double openGlHeight = openGlMazeSize.second - openGlOrigin.second;

    double horizontalScaling = openGlWidth / physicalMazeSize.first;
    double verticalScaling = openGlHeight / physicalMazeSize.second;

    std::vector<float> scalingMatrix = {
        static_cast<float>(horizontalScaling),                                 0.0, 0.0, 0.0,
                                          0.0, static_cast<float>(verticalScaling), 0.0, 0.0,
                                          0.0,                                 0.0, 1.0, 0.0,
                                          0.0,                                 0.0, 0.0, 1.0,
    };
    
    // Step 3: Construct the translation matrix. Note that here we ensure that
    // the maze is centered within the map boundaries.
    std::pair<int, int> fullMapPosition = getFullMapPosition();
    double pixelLowerLeftCornerX = fullMapPosition.first + 0.5 * (fullMapSize.first - pixelWidth);
    double pixelLowerLeftCornerY = fullMapPosition.second + 0.5 * (fullMapSize.second - pixelHeight);
    std::pair<double, double> openGlLowerLeftCorner = mapPixelCoordinateToOpenGlCoordinate(
        pixelLowerLeftCornerX, pixelLowerLeftCornerY);
    std::vector<float> translationMatrix = {
        1.0, 0.0, 0.0,  static_cast<float>(openGlLowerLeftCorner.first),
        0.0, 1.0, 0.0, static_cast<float>(openGlLowerLeftCorner.second),
        0.0, 0.0, 1.0,                                              0.0,
        0.0, 0.0, 0.0,                                              1.0,
    };

    // Step 4: Compose the matrices
    std::vector<float> transformationMatrix =
        multiply4x4Matrices(translationMatrix,
        multiply4x4Matrices(scalingMatrix,
                            initialTranslationMatrix));
    return transformationMatrix;
}

std::vector<float> GraphicUtilities::getZoomedMapTransformationMatrix(const Coordinate& initialMouseTranslation,
    const Coordinate& currentMouseTranslation, const Angle& currentMouseRotation) {

    // The purpose of this function is to produce a 4x4 matrix which,
    // when applied to the physical coordinate within the vertex shader,
    // transforms it into an OpenGL coordinate for the zoomed map. Note that
    // the zoomed map will likely not contain the entirety of the maze, so the
    // pixel coordinates will be outside of the map.

    // Step 1: Calculate the scaling matrix
    std::pair<double, double> physicalMazeSize = getPhysicalMazeSize();
    double physicalWidth = physicalMazeSize.first;
    double physicalHeight = physicalMazeSize.second;

    // Note that this is not literally the number of pixels per meter of the
    // screen. Rather, it's our desired number of pixels per simulation meter.
    double pixelsPerMeter = getScreenPixelsPerMeter() * S()->zoomedMapScale();
    double pixelWidth = pixelsPerMeter * physicalWidth;
    double pixelHeight = pixelsPerMeter * physicalHeight;

    std::pair<double, double> openGlOrigin = mapPixelCoordinateToOpenGlCoordinate(0, 0);
    std::pair<double, double> openGlMazeSize = mapPixelCoordinateToOpenGlCoordinate(pixelWidth, pixelHeight);
    double openGlWidth = openGlMazeSize.first - openGlOrigin.first;
    double openGlHeight = openGlMazeSize.second - openGlOrigin.second;

    double horizontalScaling = openGlWidth / physicalMazeSize.first;
    double verticalScaling = openGlHeight / physicalMazeSize.second;

    std::vector<float> scalingMatrix = {
        static_cast<float>(horizontalScaling),                                 0.0, 0.0, 0.0,
                                          0.0, static_cast<float>(verticalScaling), 0.0, 0.0,
                                          0.0,                                 0.0, 1.0, 0.0,
                                          0.0,                                 0.0, 0.0, 1.0,
    };

    // Step 2: Construct the translation matrix. We must ensure that the mouse
    // starts (static translation) and stays (dynamic translation) at the
    // center of the screen.

    // Part A: Find the static translation, i.e., the translation that puts the
    // center of the mouse (i.e., the midpoint of the line connecting its
    // wheels) in the center of the zoomed map. 
    std::pair<int, int> zoomedMapSize = getZoomedMapSize();
    std::pair<int, int> zoomedMapPosition = getZoomedMapPosition();
    double centerXPixels = initialMouseTranslation.getX().getMeters() * pixelsPerMeter;
    double centerYPixels = initialMouseTranslation.getY().getMeters() * pixelsPerMeter;
    double zoomedMapCenterXPixels = zoomedMapPosition.first + 0.5 * zoomedMapSize.first;
    double zoomedMapCenterYPixels = zoomedMapPosition.second + 0.5 * zoomedMapSize.second;
    double staticTranslationXPixels = zoomedMapCenterXPixels - centerXPixels;
    double staticTranslationYPixels = zoomedMapCenterYPixels - centerYPixels;
    std::pair<double, double> staticTranslation =
        mapPixelCoordinateToOpenGlCoordinate(staticTranslationXPixels, staticTranslationYPixels);

    // Part B: Find the dynamic translation, i.e., the current translation of the mouse.
    Cartesian mouseTranslationDelta = Cartesian(currentMouseTranslation) - initialMouseTranslation;
    double dynamicTranslationXPixels = mouseTranslationDelta.getX().getMeters() * pixelsPerMeter;
    double dynamicTranslationYPixels = mouseTranslationDelta.getY().getMeters() * pixelsPerMeter;
    std::pair<double, double> dynamicTranslation =
        mapPixelCoordinateToOpenGlCoordinate(dynamicTranslationXPixels, dynamicTranslationYPixels);

    // Combine the transalations and form the translation matrix
    double horizontalTranslation = staticTranslation.first -  dynamicTranslation.first + openGlOrigin.first;
    double verticalTranslation = staticTranslation.second - dynamicTranslation.second + openGlOrigin.second;
    std::vector<float> translationMatrix = {
        1.0, 0.0, 0.0, static_cast<float>(horizontalTranslation), 
        0.0, 1.0, 0.0,   static_cast<float>(verticalTranslation),
        0.0, 0.0, 1.0,                                       0.0,
        0.0, 0.0, 0.0,                                       1.0,
    };

    // Step 3: Construct a few other transformation matrices needed for
    // rotating the maze. In order to properly rotate the maze, we must first
    // translate the center of the mouse to the origin. Then we have to unscale
    // it, rotate it, scale it, and then translate it back to the proper
    // location. Hence all of the matrices.

    double theta = currentMouseRotation.getRadians();
    std::vector<float> rotationMatrix = {
        static_cast<float>( cos(theta)), static_cast<float>(sin(theta)), 0.0, 0.0,
        static_cast<float>(-sin(theta)), static_cast<float>(cos(theta)), 0.0, 0.0,
                                    0.0,                            0.0, 1.0, 0.0,
                                    0.0,                            0.0, 0.0, 1.0,
    };

    std::vector<float> inverseScalingMatrix = {
        static_cast<float>(1.0/horizontalScaling),                                     0.0, 0.0, 0.0,
                                              0.0, static_cast<float>(1.0/verticalScaling), 0.0, 0.0,
                                              0.0,                                     0.0, 1.0, 0.0,
                                              0.0,                                     0.0, 0.0, 1.0,
    };

    std::pair<double, double> zoomedMapCenterOpenGl =
        mapPixelCoordinateToOpenGlCoordinate(zoomedMapCenterXPixels, zoomedMapCenterYPixels);
    std::vector<float> translateToOriginMatrix = {
        1.0, 0.0, 0.0,  static_cast<float>(zoomedMapCenterOpenGl.first),
        0.0, 1.0, 0.0, static_cast<float>(zoomedMapCenterOpenGl.second),
        0.0, 0.0, 1.0,                                              0.0,
        0.0, 0.0, 0.0,                                              1.0,
    };

    std::vector<float> inverseTranslateToOriginMatrix = {
        1.0, 0.0, 0.0,  static_cast<float>(-zoomedMapCenterOpenGl.first),
        0.0, 1.0, 0.0, static_cast<float>(-zoomedMapCenterOpenGl.second),
        0.0, 0.0, 1.0,                                               0.0,
        0.0, 0.0, 0.0,                                               1.0,
    };

    std::vector<float> zoomedMapCameraMatrix = multiply4x4Matrices(translationMatrix, scalingMatrix);
    if (S()->rotateZoomedMap()) {
        zoomedMapCameraMatrix =
            multiply4x4Matrices(translateToOriginMatrix,
            multiply4x4Matrices(scalingMatrix,
            multiply4x4Matrices(rotationMatrix,
            multiply4x4Matrices(inverseScalingMatrix,
            multiply4x4Matrices(inverseTranslateToOriginMatrix,
                                zoomedMapCameraMatrix)))));
    }

    return zoomedMapCameraMatrix;
}

void GraphicUtilities::insertIntoGraphicCpuBuffer(const Polygon& polygon, Color color, float alpha) {
    std::vector<TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, alpha);
    for (int i = 0; i < tgs.size(); i += 1) {
        GRAPHIC_CPU_BUFFER.push_back(tgs.at(i));
    }
}

void GraphicUtilities::insertIntoTextureCpuBuffer() {
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

std::pair<int, int> GraphicUtilities::getTileGraphicTextMaxSize() {
    static std::pair<int, int> maxRowsAndCols = std::make_pair(2, 4);
    return maxRowsAndCols;
}

void GraphicUtilities::updateTileGraphicBaseColor(int x, int y, Color color) {
    int index = getTileGraphicBaseStartingIndex(x, y);
    std::tuple<float, float, float> colorValues = COLOR_TO_RGB.at(color);
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

void GraphicUtilities::updateTileGraphicWallColor(int x, int y, Direction direction, Color color, float alpha) {
    int index = getTileGraphicWallStartingIndex(x, y, direction);
    std::tuple<float, float, float> colorValues = COLOR_TO_RGB.at(color);
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

void GraphicUtilities::updateTileGraphicFog(int x, int y, float alpha) {
    int index = getTileGraphicFogStartingIndex(x, y);
    for (int i = 0; i < 2; i += 1) {
        TriangleGraphic* triangleGraphic = &GRAPHIC_CPU_BUFFER.at(index + i);
        triangleGraphic->p1.a = alpha;
        triangleGraphic->p2.a = alpha;
        triangleGraphic->p3.a = alpha;
    }
}

void GraphicUtilities::updateTileGraphicText(const Tile* tile, int numRows, int numCols, int row, int col, char c) {

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

    std::pair<int, int> maxRowsAndCols = GraphicUtilities::getTileGraphicTextMaxSize();
    Cartesian X = tile->getInteriorPolygon().getVertices().at(0);
    Cartesian Y = tile->getInteriorPolygon().getVertices().at(2);
    Cartesian diagonal = Y - X;
    Meters characterWidth = diagonal.getX() / static_cast<double>(maxRowsAndCols.second);
    Meters characterHeight = diagonal.getY() / static_cast<double>(maxRowsAndCols.first);

    // The rows start from the top and increase down the cell
    int rowsFromTop = maxRowsAndCols.first - row - 1;
    double rowOffset = rowsFromTop - static_cast<double>(maxRowsAndCols.first - numRows) / 2.0;
    double colOffset = col + static_cast<double>(maxRowsAndCols.second - numCols) / 2.0;

    int fontImageCharacterIndex = m_fontImageCharacters.find_first_of(c);
    VertexTexture p1 = { // LL
        (X + Cartesian(characterWidth *  colOffset     , characterHeight *  rowOffset     )).getX().getMeters(),
        (X + Cartesian(characterWidth *  colOffset     , characterHeight *  rowOffset     )).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex) / static_cast<double>(m_fontImageCharacters.size()),
        0.0,
    };
    VertexTexture p2 = { // UL
        (X + Cartesian(characterWidth *  colOffset     , characterHeight * (rowOffset + 1))).getX().getMeters(),
        (X + Cartesian(characterWidth *  colOffset     , characterHeight * (rowOffset + 1))).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex) / static_cast<double>(m_fontImageCharacters.size()),
        1.0,
    };
    VertexTexture p3 = { // UR
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight * (rowOffset + 1))).getX().getMeters(),
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight * (rowOffset + 1))).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex + 1) / static_cast<double>(m_fontImageCharacters.size()),
        1.0,
    };
    VertexTexture p4 = { // LR
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight *  rowOffset     )).getX().getMeters(),
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight *  rowOffset     )).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex + 1) / static_cast<double>(m_fontImageCharacters.size()),
        0.0,
    };

    int triangleTextureIndex = getTileGraphicTextStartingIndex(tile->getX(), tile->getY(), row, col);
    TEXTURE_CPU_BUFFER.at(triangleTextureIndex) = {p1, p2, p3};
    TEXTURE_CPU_BUFFER.at(triangleTextureIndex + 1) = {p1, p3, p4};
}

void GraphicUtilities::drawMousePolygon(const Polygon& polygon, Color color, float sensorAlpha) {
    std::vector<TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, sensorAlpha);
    GRAPHIC_CPU_BUFFER.insert(GRAPHIC_CPU_BUFFER.end(), tgs.begin(), tgs.end());
}

double GraphicUtilities::getScreenPixelsPerMeter() {
    // TODO: MACK - make this part of initialization in the main function...
    // Assumptions:
    // 1) These values will not change during an execution of the program
    // 2) Monitor pixels are square (and thus the pixels per meter is the same
    //    for both the horizontal and vertical direcitons).
    static double pixels = glutGet(GLUT_SCREEN_WIDTH);
    static double millimeters = glutGet(GLUT_SCREEN_WIDTH_MM);
    static double pixelsPerMeter = 1000 * pixels / millimeters;
    return pixelsPerMeter;
}

std::pair<double, double> GraphicUtilities::getPhysicalMazeSize() {
    double width = P()->wallWidth() + m_mazeWidth * (P()->wallWidth() + P()->wallLength());
    double height = P()->wallWidth() + m_mazeHeight * (P()->wallWidth() + P()->wallLength());
    return std::make_pair(width, height);
}

std::vector<float> GraphicUtilities::multiply4x4Matrices(std::vector<float> left, std::vector<float> right) {
    ASSERT_EQUAL(left.size(), 16);
    ASSERT_EQUAL(right.size(), 16);
    std::vector<float> result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double value = 0.0;
            for (int k = 0; k < 4; k++) {
                value += left.at(4*i+k) * right.at(4*k+j);
            }
            result.push_back(value);
        }
    }
    ASSERT_EQUAL(result.size(), 16);
    return result;
}

std::pair<double, double> GraphicUtilities::mapPixelCoordinateToOpenGlCoordinate(double x, double y) {
    return std::make_pair(2 * x / m_windowWidth - 1, 2 * y / m_windowHeight - 1);
}

std::vector<TriangleGraphic> GraphicUtilities::polygonToTriangleGraphics(const Polygon& polygon, Color color, float alpha) {
    std::vector<Triangle> triangles = polygon.triangulate();
    std::vector<TriangleGraphic> triangleGraphics;
    // TODO: MACK 3 - can these be doubles?
    std::tuple<float, float, float> colorValues = COLOR_TO_RGB.at(color);
    for (Triangle triangle : triangles) {
        triangleGraphics.push_back({
            {triangle.getP1().getX().getMeters(), triangle.getP1().getY().getMeters(),
                std::get<0>(colorValues), std::get<1>(colorValues), std::get<2>(colorValues), alpha},
            {triangle.getP2().getX().getMeters(), triangle.getP2().getY().getMeters(),
                std::get<0>(colorValues), std::get<1>(colorValues), std::get<2>(colorValues), alpha},
            {triangle.getP3().getX().getMeters(), triangle.getP3().getY().getMeters(),
                std::get<0>(colorValues), std::get<1>(colorValues), std::get<2>(colorValues), alpha}});
    }
    return triangleGraphics;
}

int GraphicUtilities::trianglesPerTile() {
    // This value must be predetermined, and was done so as follows:
    // Base polygon:      2 (2 triangles x 1 polygon  per tile)
    // Wall polygon:      8 (2 triangles x 4 polygons per tile)
    // Corner polygon:    8 (2 triangles x 4 polygons per tile)
    // Fog polygon:       2 (2 triangles x 1 polygon  per tile)
    // --------------------
    // Total             20
    return 20;
}

int GraphicUtilities::getTileGraphicBaseStartingIndex(int x, int y) {
    return  0 + trianglesPerTile() * (m_mazeHeight * x + y);
}

int GraphicUtilities::getTileGraphicWallStartingIndex(int x, int y, Direction direction) {
    return  2 + trianglesPerTile() * (m_mazeHeight * x + y) + (2 * SimUtilities::getDirectionIndex(direction));
}

int GraphicUtilities::getTileGraphicCornerStartingIndex(int x, int y, int cornerNumber) {
    return 10 + trianglesPerTile() * (m_mazeHeight * x + y) + (2 * cornerNumber);
}

int GraphicUtilities::getTileGraphicFogStartingIndex(int x, int y) {
    return 18 + trianglesPerTile() * (m_mazeHeight * x + y);
}

int GraphicUtilities::getTileGraphicTextStartingIndex(int x, int y, int row, int col) {
    static std::pair<int, int> maxRowsAndCols = getTileGraphicTextMaxSize();
    static int triangleTexturesPerTile = 2 * maxRowsAndCols.first * maxRowsAndCols.second;
    return triangleTexturesPerTile * (m_mazeHeight * x + y) + 2 * (row * maxRowsAndCols.second + col);
}

} // namespace sim

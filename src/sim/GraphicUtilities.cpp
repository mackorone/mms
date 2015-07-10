#include "GraphicUtilities.h"

#include <algorithm>
#include <glut.h>

#include "Assert.h"
#include "GeometryUtilities.h"
#include "Param.h"
#include "SimUtilities.h"
#include "Triangle.h"

#include <iostream>

namespace sim {

// Here we have to explicity create the TGB, since we only declared it in the header
std::vector<TriangleGraphic> GraphicUtilities::TGB;

// Same goes for the static maze height
int GraphicUtilities::m_mazeWidth = 0;
int GraphicUtilities::m_mazeHeight = 0;
void GraphicUtilities::setMazeSize(int mazeWidth, int mazeHeight) {
    ASSERT(m_mazeWidth == 0 && m_mazeHeight == 0);
    ASSERT(0 < mazeWidth && 0 < mazeHeight);
    m_mazeWidth = mazeWidth;
    m_mazeHeight = mazeHeight;
}

std::vector<float> GraphicUtilities::getFullMapCameraMatrix() {

    // First, we calculate the stretch factor in both the horizontal in vertical directions.
    // That is, we need to find the ratio of units per meter so that way we can transform
    // physical dimensions into OpenGL dimensions.

    // Horizontal and vertical (respectively) OpenGL units per pixel
    float horizontalUnitsPerPixel = 2.0 / P()->windowWidth();
    float verticalUnitsPerPixel = 2.0 / P()->windowHeight();

    // Maze width and height (respectively) in OpenGL units
    float mazeWidthUnits = P()->fullMapWidth() * horizontalUnitsPerPixel;
    float mazeHeightUnits = P()->fullMapHeight() * verticalUnitsPerPixel;

    // The maze width and height in meters
    float mazeWidth = P()->wallWidth() + m_mazeWidth * (P()->wallWidth() + P()->wallLength());
    float mazeHeight = P()->wallWidth() + m_mazeHeight * (P()->wallWidth() + P()->wallLength());

    // Horizontal and vertical (respectively) OpenGL units per meter
    float horizontalUnitsPerMeter = mazeWidthUnits / mazeWidth;
    float verticalUnitsPerMeter = mazeHeightUnits / mazeHeight;

    // Next, we calculate the translation
    float horizontalTranslation = P()->fullMapPositionX() * horizontalUnitsPerPixel - 1;
    float verticalTranslation = P()->fullMapPositionY() * verticalUnitsPerPixel - 1;

    std::vector<float> fullMapCameraMatrix = {
        horizontalUnitsPerMeter, 0.0, 0.0, horizontalTranslation,
        0.0, verticalUnitsPerMeter, 0.0, verticalTranslation,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0,
    };

    ASSERT(fullMapCameraMatrix.size() == 16);
    return fullMapCameraMatrix;
}

std::vector<float> GraphicUtilities::getZoomedMapCameraMatrix(const Coordinate& initialMouseTranslation,
    const Coordinate& currentMouseTranslation, const Angle& currentMouseRotation) {

    // First, we calculate the stretch factor in both the horizontal in vertical directions.
    // That is, we need to find the ratio of units per meter so that way we can transform
    // physical dimensions into OpenGL dimensions.

    // Horizontal and vertical (respectively) OpenGL units per pixel
    float horizonalUnitsPerPixel = 2.0 / P()->windowWidth();
    float verticalUnitsPerPixel = 2.0 / P()->windowHeight();

    // Maze width and height (respectively) in OpenGL units
    float mazeWidthUnits = P()->zoomedMapWidth() * horizonalUnitsPerPixel * P()->zoomFactor();
    float mazeHeightUnits = P()->zoomedMapHeight() * verticalUnitsPerPixel * P()->zoomFactor();

    // The maze width and height in meters
    float mazeWidth = P()->wallWidth() + m_mazeWidth * (P()->wallWidth() + P()->wallLength());
    float mazeHeight = P()->wallWidth() + m_mazeHeight * (P()->wallWidth() + P()->wallLength());

    // Horizontal and vertical (respectively) OpenGL units per meter
    float horizontalUnitsPerMeter = mazeWidthUnits / mazeWidth;
    float verticalUnitsPerMeter = mazeHeightUnits / mazeHeight;

    // Next, we need to calculate the translation. This is done as a two-fold process. First, we find
    // the translation that we need to perform to put the center of the first square in the center of
    // the zoomed map. This is called the static translation. Then we find the translation of the mouse,
    // which is called the dynamic translation. When we add those two translations together, we ensure
    // that the mouse starts (static) and stays (dynamic) at the center of the screen.

    // Middle of the map in the horizontal and vertical directions (respectively) in pixels
    float horizontalMapCenterPixels = P()->zoomedMapPositionX() + 0.5 * P()->zoomedMapWidth();
    float verticalMapCenterPixels = P()->zoomedMapPositionY() + 0.5 * P()->zoomedMapHeight();

    // Find the static translation (all in OpenGL units)
    float startHorizontalMapCenter = mazeWidthUnits * ((2 * P()->wallWidth() + P()->wallLength()) / 2.0) / mazeWidth;
    float startVerticalMapCenter = mazeHeightUnits * ((2 * P()->wallWidth() + P()->wallLength()) / 2.0) / mazeHeight;
    float endHorizontalMapCenter = horizontalMapCenterPixels * horizonalUnitsPerPixel - 1;
    float endVerticalMapCenter = verticalMapCenterPixels * verticalUnitsPerPixel - 1;
    float staticHorizontalTranslation = endHorizontalMapCenter - startHorizontalMapCenter;
    float staticVerticalTranslation = endVerticalMapCenter - startVerticalMapCenter;

    // Find the dynamic translation (first in physical units, then in OpenGL units)
    Cartesian mouseTranslationDelta = Cartesian(initialMouseTranslation) - currentMouseTranslation;
    float dynamicHorizontalTranslation = (mouseTranslationDelta.getX().getMeters() / mazeWidth) * mazeWidthUnits;
    float dynamicVerticalTranslation = (mouseTranslationDelta.getY().getMeters() / mazeHeight) * mazeHeightUnits;

    // Get the total translation (in OpenGL units)
    float horizontalTranslation = staticHorizontalTranslation + dynamicHorizontalTranslation;
    float verticalTranslation = staticVerticalTranslation + dynamicVerticalTranslation;

    // TODO: Rotation

    std::vector<float> zoomedMapCameraMatrix = {
        horizontalUnitsPerMeter, 0.0, 0.0, horizontalTranslation,
        0.0, verticalUnitsPerMeter, 0.0, verticalTranslation,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0,
    };
    ASSERT(zoomedMapCameraMatrix.size() == 16);
    return zoomedMapCameraMatrix;
}

void GraphicUtilities::drawTileGraphicBase(int x, int y, const Polygon& polygon, const GLfloat* color) {
    std::vector<sim::TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, 1.0);
    int index = getTileGraphicBaseStartingIndex(x, y);
    for (int i = 0; i < tgs.size(); i += 1) {
        if (index + i < TGB.size()) {
            TGB.at(index + i) = tgs.at(i);
        }
        else {
            TGB.push_back(tgs.at(i));
        }
    }
}

void GraphicUtilities::drawTileGraphicWall(int x, int y, Direction direction, const Polygon& polygon,
        const GLfloat* color, GLfloat alpha) {
    std::vector<sim::TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, alpha);
    int index = getTileGraphicWallStartingIndex(x, y, direction);
    for (int i = 0; i < tgs.size(); i += 1) {
        if (index + i < TGB.size()) {
            TGB.at(index + i) = tgs.at(i);
        }
        else {
            TGB.push_back(tgs.at(i));
        }
    }
}

void GraphicUtilities::drawTileGraphicCorner(int x, int y, int cornerNumber, const Polygon& polygon,
        const GLfloat* color) {
    std::vector<sim::TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, 1.0);
    int index = getTileGraphicCornerStartingIndex(x, y, cornerNumber);
    for (int i = 0; i < tgs.size(); i += 1) {
        if (index + i < TGB.size()) {
            TGB.at(index + i) = tgs.at(i);
        }
        else {
            TGB.push_back(tgs.at(i));
        }
    }
}

void GraphicUtilities::drawTileGraphicFog(int x, int y, const Polygon& polygon, const GLfloat* color, GLfloat alpha) {
    std::vector<sim::TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, alpha);
    int index = getTileGraphicFogStartingIndex(x, y);
    for (int i = 0; i < tgs.size(); i += 1) {
        if (index + i < TGB.size()) {
            TGB.at(index + i) = tgs.at(i);
        }
        else {
            TGB.push_back(tgs.at(i));
        }
    }
}

void GraphicUtilities::updateTileGraphicBaseColor(int x, int y, const GLfloat* color) {
    int index = getTileGraphicBaseStartingIndex(x, y);
    for (int i = 0; i < 2; i += 1) {
        TriangleGraphic* triangleGraphic = &TGB.at(index + i);
        triangleGraphic->p1.r = color[0];
        triangleGraphic->p1.g = color[1];
        triangleGraphic->p1.b = color[2];
        triangleGraphic->p2.r = color[0];
        triangleGraphic->p2.g = color[1];
        triangleGraphic->p2.b = color[2];
        triangleGraphic->p3.r = color[0];
        triangleGraphic->p3.g = color[1];
        triangleGraphic->p3.b = color[2];
    }
}

void GraphicUtilities::updateTileGraphicWallColor(int x, int y, Direction direction, const GLfloat* color, GLfloat alpha) {
    int index = getTileGraphicWallStartingIndex(x, y, direction);
    for (int i = 0; i < 2; i += 1) {
        TriangleGraphic* triangleGraphic = &TGB.at(index + i);
        triangleGraphic->p1.r = color[0];
        triangleGraphic->p1.g = color[1];
        triangleGraphic->p1.b = color[2];
        triangleGraphic->p1.a = alpha;
        triangleGraphic->p2.r = color[0];
        triangleGraphic->p2.g = color[1];
        triangleGraphic->p2.b = color[2];
        triangleGraphic->p2.a = alpha;
        triangleGraphic->p3.r = color[0];
        triangleGraphic->p3.g = color[1];
        triangleGraphic->p3.b = color[2];
        triangleGraphic->p3.a = alpha;
    }
}

void GraphicUtilities::updateTileGraphicFog(int x, int y, GLfloat alpha) {
    int index = getTileGraphicFogStartingIndex(x, y);
    for (int i = 0; i < 2; i += 1) {
        TriangleGraphic* triangleGraphic = &TGB.at(index + i);
        triangleGraphic->p1.a = alpha;
        triangleGraphic->p2.a = alpha;
        triangleGraphic->p3.a = alpha;
    }
}

void GraphicUtilities::drawMousePolygon(const Polygon& polygon, const GLfloat* color, float sensorAlpha) {
    std::vector<sim::TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, sensorAlpha);
    TGB.insert(TGB.end(), tgs.begin(), tgs.end());
}

void GraphicUtilities::drawText(const Coordinate& location, const Distance& width, const Distance& height, const std::string& text) {

    // TODO: Performance issues - avoid immediate mode
    // TODO: Ideally, we should be able to *not* have this, provided we optimized correctly

    /*
    // If there is no text, we don't have to do anything
    if (SimUtilities::trim(text).empty()) {
        return;
    }

    // First, get the width of the text in pixels
    float pixelWidth = width.getMeters() * P()->pixelsPerMeter();
    float pixelHeight = height.getMeters() * P()->pixelsPerMeter();

    // Next, determine the scale the text using the window dimensions. As specified in the
    // documentation for glutStrokeCharacter, the GLUT_STROKE_MONO_ROMAN has characters
    // that are each exactly 104.76 units wide, hence the use of the literal value.
    std::pair<int, int> windowSize = getInitialWindowSize();
    float scaleX = 1.0/104.76 * (pixelWidth / windowSize.first) / text.size() * 2;
    float scaleY = 1.0/104.76 * (pixelHeight / windowSize.second) * 2;

    // After, get the OpenGL location of the text
    std::pair<float, float> coordinates = getOpenGlCoordinates(location);

    // Finally, draw the text
    glPushMatrix();
    glScalef(scaleX, scaleY, 0);
    glTranslatef(coordinates.first * 1.0/scaleX, coordinates.second * 1.0/scaleY, 0);
    for (int i = 0; i < text.size(); i += 1) {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, text.at(i));
    }
    glPopMatrix();
    */
}

// TODO: delete this
std::pair<float, float> GraphicUtilities::mapCoordinateToOpenGlCoordinates(const Coordinate& coordinate) {
    /*
    float pixelCoodinateX = P()->mapPositionX() + P()->mapWidth() * getFractionOfDistance(
        -0.5 * P()->wallWidth(), 0.5 * P()->wallWidth() + m_mazeWidth * (P()->wallWidth() + P()->wallLength()),
        coordinate.getX().getMeters());
    float pixelCoodinateY = P()->mapPositionY() + P()->mapHeight() * getFractionOfDistance(
        -0.5 * P()->wallWidth(), 0.5 * P()->wallWidth() + m_mazeHeight * (P()->wallWidth() + P()->wallLength()),
        coordinate.getY().getMeters());
    float openGlCoordinateX = ((pixelCoodinateX / P()->windowWidth()) - 0.5) * 2;
    float openGlCoordinateY = ((pixelCoodinateY / P()->windowHeight()) - 0.5) * 2;
    */
    return std::make_pair(coordinate.getX().getMeters(), coordinate.getY().getMeters());
}

// TODO: Do I need this???
float GraphicUtilities::getFractionOfDistance(float start, float end, float location) {
    return (location - start) / (end - start);
}

std::vector<TriangleGraphic> GraphicUtilities::polygonToTriangleGraphics(const Polygon& polygon, const GLfloat* color, GLfloat alpha) {
    std::vector<Triangle> triangles = GeometryUtilities::triangulate(polygon);
    std::vector<TriangleGraphic> triangleGraphics;
    for (Triangle triangle : triangles) {
        triangleGraphics.push_back({
            {triangle.getP1().getX().getMeters(), triangle.getP1().getY().getMeters(), color[0], color[1], color[2], alpha},
            {triangle.getP2().getX().getMeters(), triangle.getP2().getY().getMeters(), color[0], color[1], color[2], alpha},
            {triangle.getP3().getX().getMeters(), triangle.getP3().getY().getMeters(), color[0], color[1], color[2], alpha}});
    }
    return triangleGraphics;
}

int GraphicUtilities::trianglesPerTile() {
    // This value must be predetermined, and was done so as follows:
    // - Base polygon: 2 triangles
    // - Wall polygon: 2 triangles each
    // - Corner polygon: 2 triangles each
    // - Fog polygon: 2 triangles 
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

} // namespace sim

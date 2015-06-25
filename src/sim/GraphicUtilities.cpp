#include "GraphicUtilities.h"

#include <algorithm>
#include <glut.h>

#include "Assert.h"
#include "GeometryUtilities.h"
#include "Param.h"
#include "SimUtilities.h"
#include "Triangle.h"

namespace sim {

// Here we have to explicity create the TGB, since we only declared it in the header
std::vector<TriangleGraphic> GraphicUtilities::TGB;

// Same goes for the static maze height
int GraphicUtilities::s_mazeHeight = 0;
void GraphicUtilities::setMazeHeight(int height) {
    ASSERT(s_mazeHeight == 0);
    s_mazeHeight = height;
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

void GraphicUtilities::drawMousePolygon(const Polygon& polygon, const GLfloat* color) {
    std::vector<sim::TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, 1);
    TGB.insert(TGB.end(), tgs.begin(), tgs.end());
}

void GraphicUtilities::drawText(const Coordinate& location, const Distance& width, const Distance& height, const std::string& text) {

    // TODO: Performance issues - avoid immediate mode
    // TODO: Ideally, we should be able to *not* have this, provided we optimized correctly

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
    std::pair<int, int> windowSize = getWindowSize();
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
}

std::pair<int, int> GraphicUtilities::getWindowSize() {
    // TODO: MACK this shouldn't work... make the window size a state variable that updates when the size changes
    // "glutGet" is expensive so we only make the call once
    // TODO: Is this initial window size????
    static const int width = glutGet(GLUT_WINDOW_WIDTH);
    static const int height = glutGet(GLUT_WINDOW_HEIGHT);
    return std::make_pair(width, height);
}

std::pair<float, float> GraphicUtilities::getOpenGlCoordinates(const Coordinate& coordinate) {
    std::pair<int, int> windowSize = getWindowSize();
    float pixelCoodinateX = coordinate.getX().getMeters() * P()->pixelsPerMeter();
    float pixelCoodinateY = coordinate.getY().getMeters() * P()->pixelsPerMeter();
    float openGlCoordinateX = ((pixelCoodinateX / windowSize.first) - 0.5) * 2;
    float openGlCoordinateY = ((pixelCoodinateY / windowSize.second) - 0.5) * 2;
    return std::make_pair(openGlCoordinateX, openGlCoordinateY);
}

std::vector<TriangleGraphic> GraphicUtilities::polygonToTriangleGraphics(const Polygon& polygon, const GLfloat* color, GLfloat alpha) {
    std::vector<Triangle> triangles = GeometryUtilities::triangulate(polygon);
    std::vector<TriangleGraphic> triangleGraphics;
    for (Triangle triangle : triangles) {
        std::pair<float, float> p1 = getOpenGlCoordinates(triangle.getP1());
        std::pair<float, float> p2 = getOpenGlCoordinates(triangle.getP2());
        std::pair<float, float> p3 = getOpenGlCoordinates(triangle.getP3());
        triangleGraphics.push_back({{p1.first, p1.second, color[0], color[1], color[2], alpha},
                                    {p2.first, p2.second, color[0], color[1], color[2], alpha},
                                    {p3.first, p3.second, color[0], color[1], color[2], alpha}});
    }
    return triangleGraphics;
}

int GraphicUtilities::trianglesPerTile() {
    // TODO
    return 20;
}

// TODO: These indices are dependent on the order in which they're initially drawn: Make a note of this somewhere

int GraphicUtilities::getTileGraphicBaseStartingIndex(int x, int y) {
    return  0 + trianglesPerTile() * (s_mazeHeight * x + y);
}

int GraphicUtilities::getTileGraphicWallStartingIndex(int x, int y, Direction direction) {
    return  2 + trianglesPerTile() * (s_mazeHeight * x + y) + 2 * direction;
    //2 * (std::find(DIRECTIONS.begin(), DIRECTIONS.end(), direction) - DIRECTIONS.begin()); // TODO: more flexible
}

int GraphicUtilities::getTileGraphicCornerStartingIndex(int x, int y, int cornerNumber) {
    return 10 + trianglesPerTile() * (s_mazeHeight * x + y) + (2 * cornerNumber);
}

int GraphicUtilities::getTileGraphicFogStartingIndex(int x, int y) {
    return 18 + trianglesPerTile() * (s_mazeHeight * x + y);
}

} // namespace sim

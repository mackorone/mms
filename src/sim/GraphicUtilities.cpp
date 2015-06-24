#include "GraphicUtilities.h"

#include <glut.h>

#include "GeometryUtilities.h" // TODO
#include "SimUtilities.h" // TODO
#include "Triangle.h"

#include <iostream> // TODO

#include "Param.h"

extern std::vector<sim::TriangleGraphic> triangleGraphics;

namespace sim {

void GraphicUtilities::updateTileGraphicBase(int x, int y, const Polygon& polygon, const GLfloat* color, GLfloat alpha) {
    std::vector<sim::TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, alpha);

    // TODO
    int index = 20 * (16 * x + y); // Number of triangle objects prior
    for (int i = 0; i < tgs.size(); i += 1) {
        if (index + i < triangleGraphics.size()) {
            triangleGraphics.at(index + i) = tgs.at(i);
        }
        else {
            triangleGraphics.push_back(tgs.at(i));
        }
    }
}

void GraphicUtilities::updateTileGraphicWall(int x, int y, Direction direction, const Polygon& polygon,
        const GLfloat* color, GLfloat alpha) {
    std::vector<sim::TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, alpha);
    // TODO
    int index = 20 * (16 * x + y) + 2 + (2 * direction); // Number of triangle objects prior
    for (int i = 0; i < tgs.size(); i += 1) {
        if (index + i < triangleGraphics.size()) {
            triangleGraphics.at(index + i) = tgs.at(i);
        }
        else {
            triangleGraphics.push_back(tgs.at(i));
        }
    }
}

void GraphicUtilities::updateTileGraphicCorner(int x, int y, int cornerNumber, const Polygon& polygon,
        const GLfloat* color, GLfloat alpha) {
    std::vector<sim::TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, alpha);
    // TODO

    int index = 20 * (16 * x + y) + 10 + (2 * cornerNumber); // Number of triangle objects prior
    for (int i = 0; i < tgs.size(); i += 1) {
        if (index + i < triangleGraphics.size()) {
            triangleGraphics.at(index + i) = tgs.at(i);
        }
        else {
            triangleGraphics.push_back(tgs.at(i));
        }
    }
}

void GraphicUtilities::updateTileGraphicFog(int x, int y, const Polygon& polygon, const GLfloat* color, GLfloat alpha) {
    std::vector<sim::TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, alpha);
    // TODO
    int index = 20 * (16 * x + y) + 18; // Number of triangle objects prior
    for (int i = 0; i < tgs.size(); i += 1) {
        if (index + i < triangleGraphics.size()) {
            triangleGraphics.at(index + i) = tgs.at(i);
        }
        else {
            triangleGraphics.push_back(tgs.at(i));
        }
    }
}

void GraphicUtilities::drawMousePolygon(const Polygon& polygon, const GLfloat* color, GLfloat alpha) {
    std::vector<sim::TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, alpha);
    triangleGraphics.insert(triangleGraphics.end(), tgs.begin(), tgs.end());
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
    // TODO: This method should only need to be called once per polygon...
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

} // namespace sim

#include "GraphicUtilities.h"

#include <glut.h>
#include "SimUtilities.h" // TODO

#include "Param.h"

namespace sim {

void GraphicUtilities::drawPolygon(const Polygon& polygon) {

    // TODO: Performance issues - avoid immediate mode

    // TODO: SOM - GL_POLYGON can only draw convex polygons
    // Reimplement this (with a tesselator) to draw concave ones

    // First we have to convert the physical coordinates to pixel coordinates.
    // Then we have convert the pixel coordinates to OpenGL coordinates.
    glBegin(GL_POLYGON);
    for (Cartesian vertex : polygon.getVertices()) {
        std::pair<float, float> coordinates = getOpenGlCoordinates(vertex);
        glVertex2f(coordinates.first, coordinates.second);
    }
    glEnd();
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
    // "glutGet" is expensive so we only make the call once
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

} // namespace sim

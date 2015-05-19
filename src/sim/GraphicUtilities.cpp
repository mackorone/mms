#include "GraphicUtilities.h"

#include <glut.h>

#include "Param.h"

namespace sim {

void GraphicUtilities::drawPolygon(const Polygon& polygon) {

    // TODO: SOM - GL_POLYGON can only draw convex polygons
    // Reimplement this (with a tesselator) to draw concave ones

    // "glutGet" is expensive so we only make the call once
    static const int WINDOW_WIDTH = glutGet(GLUT_WINDOW_WIDTH);
    static const int WINDOW_HEIGHT = glutGet(GLUT_WINDOW_HEIGHT);

    // First we have to convert the physical coordinates to pixel coordinates.
    // Then we have convert the pixel coordinates to OpenGL coordinates.
    glBegin(GL_POLYGON);
    for (Cartesian vertex : polygon.getVertices()) {
        float pixelCoodinateX = vertex.getX().getMeters() * P()->pixelsPerMeter();
        float pixelCoodinateY = vertex.getY().getMeters() * P()->pixelsPerMeter();
        float openGlCoordinateX = ((pixelCoodinateX / WINDOW_WIDTH) - 0.5) * 2;
        float openGlCoordinateY = ((pixelCoodinateY / WINDOW_HEIGHT) - 0.5) * 2;
        glVertex2f(openGlCoordinateX, openGlCoordinateY);
    }
    glEnd();
}

} // namespace sim

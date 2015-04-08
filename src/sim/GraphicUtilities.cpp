#include "GraphicUtilities.h"

#include <glut.h>

#include "Param.h"

namespace sim {

void drawPolygon(const Polygon& polygon) {

    // TODO: SOM
    // GL_POLYGON can only draw convex polygons. Reimplement this (with a tesselator)
    // to draw concave ones

    // First we have to convert the physical coordinates to pixel coordinates.
    // Then we have convert the pixel coordinates to OpenGL coordinates.
    glBegin(GL_POLYGON);
    for (Cartesian vertex : polygon.getVertices()) {
        float pixelCoodinateX = vertex.getX().getMeters() * P()->pixelsPerMeter();
        float pixelCoodinateY = vertex.getY().getMeters() * P()->pixelsPerMeter();
        float openGlCoordinateX = ((pixelCoodinateX / P()->windowWidth()) - 0.5) * 2;
        float openGlCoordinateY = ((pixelCoodinateY / P()->windowHeight()) - 0.5) * 2;
        glVertex2f(openGlCoordinateX, openGlCoordinateY);
    }
    glEnd();
}

} // namespace sim

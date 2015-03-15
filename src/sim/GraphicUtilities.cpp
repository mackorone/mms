#include "GraphicUtilities.h"

#include <GL/freeglut.h>

#include "Param.h"

namespace sim {

void drawPolygon(const Polygon& polygon) {

    // We only need to call these once
    static float pixelsPerMeter = P()->pixelsPerMeter();
    static float windowWidth = P()->windowWidth();
    static float windowHeight = P()->windowHeight();

    // First we have to convert the physical coordinates to pixel coordinates.
    // Then we have convert the pixel coordinates to OpenGL coordinates.
    glBegin(GL_POLYGON);
    for (Cartesian vertex : polygon.getVertices()) {
        float pixelCoodinateX = vertex.getX().getMeters() * pixelsPerMeter;
        float pixelCoodinateY = vertex.getY().getMeters() * pixelsPerMeter;
        float openGlCoordinateX = ((pixelCoodinateX / windowWidth) - 0.5) * 2;
        float openGlCoordinateY = ((pixelCoodinateY / windowHeight) - 0.5) * 2;
        glVertex2f(openGlCoordinateX, openGlCoordinateY);
    }
    glEnd();
}

} // namespace sim

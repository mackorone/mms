#include "TileGraphic.h"

#include "Colors.h"
#include "Utilities.h"

namespace sim {

TileGraphic::TileGraphic(Tile* tile) : m_tile(tile) {
}

void TileGraphic::draw() {

    // Draw the base of the tile
    glColor3fv(BLACK);
    drawPolygon(m_tile->getBasePolygon());

    // Draw the walls of the tile
    glColor3fv(RED);
    for (Polygon polygon : m_tile->getWallPolygons()) {
        drawPolygon(polygon);
    }

    // Draw the corners of the tile
    glColor3fv(LIGHTGRAY);
    for (Polygon polygon : m_tile->getCornerPolygons()) {
        drawPolygon(polygon);
    }
}

} // namespace sim

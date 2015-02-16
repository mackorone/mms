#include "TileGraphic.h"

#include "Colors.h"
#include "Param.h"
#include "Utilities.h"

namespace sim {

TileGraphic::TileGraphic(Tile* tile) : m_tile(tile) {
}

void TileGraphic::draw() {

    // Draw the base of the tile
    glColor3fv(COLORS.at(P()->tileBaseColor()));
    drawPolygon(m_tile->getBasePolygon());

    // Draw the walls of the tile
    glColor3fv(COLORS.at(P()->tileWallColor()));
    for (Polygon polygon : m_tile->getWallPolygons()) {
        drawPolygon(polygon);
    }

    // Draw the corners of the tile
    glColor3fv(COLORS.at(P()->tileCornerColor()));
    for (Polygon polygon : m_tile->getCornerPolygons()) {
        drawPolygon(polygon);
    }
}

} // namespace sim

#include "TileGraphic.h"

#include "Colors.h"
#include "Param.h"
#include "GraphicUtilities.h"

namespace sim {

TileGraphic::TileGraphic(const Tile* tile) : m_tile(tile), m_color(COLORS.at(P()->tileBaseColor())) {
}

void TileGraphic::draw() const {

    // Draw the base of the tile
    glColor3fv(m_color);
    drawPolygon(m_tile->getFullPolygon());

    // Draw the walls of the tile
    glColor3fv(COLORS.at(P()->tileWallColor()));
    for (Polygon polygon : m_tile->getActualWallPolygons()) {
        drawPolygon(polygon);
    }

    // Draw the corners of the tile
    glColor3fv(COLORS.at(P()->tileCornerColor()));
    for (Polygon polygon : m_tile->getCornerPolygons()) {
        drawPolygon(polygon);
    }
}

void TileGraphic::setColor(const GLfloat* color) {
    m_color = color;
}

} // namespace sim

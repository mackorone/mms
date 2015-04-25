#include "TileGraphic.h"

#include "Colors.h"
#include "GraphicUtilities.h"
#include "Param.h"
#include "State.h"

namespace sim {

TileGraphic::TileGraphic(const Tile* tile) : m_tile(tile), m_color(COLOR_STRINGS.at(P()->tileBaseColor())) {
    for (Direction direction : DIRECTIONS) {
        m_algoWalls[direction] = false;
    }
}

void TileGraphic::draw() const {

    // Draw the base of the tile
    glColor3fv(S()->tileColorsVisible() ? m_color : COLOR_STRINGS.at(P()->tileBaseColor()));
    GraphicUtilities::drawPolygon(m_tile->getFullPolygon());

    // Either draw the true walls of the tile
    glColor3fv(COLOR_STRINGS.at(P()->tileWallColor()));
    if (S()->wallTruthVisible()) {
        for (Polygon polygon : m_tile->getActualWallPolygons()) {
            GraphicUtilities::drawPolygon(polygon);
        }
    }
    else { // Or the algorithms declared walls
        for (Direction direction : DIRECTIONS) {
            if (m_algoWalls.at(direction)) {
                GraphicUtilities::drawPolygon(m_tile->getWallPolygon(direction));
            }
        }
    }

    // Draw the corners of the tile
    glColor3fv(COLOR_STRINGS.at(P()->tileCornerColor()));
    for (Polygon polygon : m_tile->getCornerPolygons()) {
        GraphicUtilities::drawPolygon(polygon);
    }

    // TODO: Draw the shortest path graphic, which is usefule because this is
    // the metric by which mice that don't reach the center are ranked
}

void TileGraphic::setColor(const GLfloat* color) {
    m_color = color;
}

bool TileGraphic::getAlgoWall(Direction direction) const {
    return m_algoWalls.at(direction);
}

void TileGraphic::setAlgoWall(Direction direction, bool isWall) {
    m_algoWalls[direction] = isWall;
}

} // namespace sim

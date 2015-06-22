#include "TileGraphic.h"

#include "Colors.h"
#include "GraphicUtilities.h"
#include "Param.h"
#include "State.h"

namespace sim {

TileGraphic::TileGraphic(const Tile* tile) : m_tile(tile), m_color(COLOR_STRINGS.at(P()->tileBaseColor())),
        m_text(""), m_foggy(true) {
}

void TileGraphic::draw() const {

    // Draw the base of the tile
    const GLfloat* baseColor = S()->tileColorsVisible() ? m_color : COLOR_STRINGS.at(P()->tileBaseColor());
    GraphicUtilities::drawPolygon(m_tile->getFullPolygon(), baseColor, 1.0);

    // Either draw the true walls of the tile ...
    if (S()->wallTruthVisible()) {
        for (Polygon polygon : m_tile->getActualWallPolygons()) {
            GraphicUtilities::drawPolygon(polygon, COLOR_STRINGS.at(P()->tileWallColor()), 1.0);
        }
    }

    // ... or the algorithm's declared walls
    else {
        for (Direction direction : DIRECTIONS) {

            // Declare the wall color
            const GLfloat* wallColor;

            // If the wall was declared, use the wall color and tile base color ...
            if (m_declaredWalls.find(direction) != m_declaredWalls.end()) {
                if (m_declaredWalls.at(direction)) {
                    if (m_tile->isWall(direction)) {
                        wallColor = COLOR_STRINGS.at(P()->tileWallColor());
                    }
                    else {
                        wallColor = COLOR_STRINGS.at(P()->tileIncorrectlyDeclaredWallColor());
                    }
                }
                else {
                    if (m_tile->isWall(direction)) {
                        wallColor = COLOR_STRINGS.at(P()->tileIncorrectlyDeclaredNoWallColor());
                    }
                    else {
                        wallColor = baseColor;
                    }
                }
            }

            // ... otherwise, use the undeclared walls colors
            else {
                if (m_tile->isWall(direction)) {
                    wallColor = COLOR_STRINGS.at(P()->tileUndeclaredWallColor());
                }
                else {
                    wallColor = COLOR_STRINGS.at(P()->tileUndeclaredNoWallColor());
                }
            }

            GraphicUtilities::drawPolygon(m_tile->getWallPolygon(direction), wallColor, 1.0);
        }
    }

    // Draw the corners of the tile
    for (Polygon polygon : m_tile->getCornerPolygons()) {
        GraphicUtilities::drawPolygon(polygon, COLOR_STRINGS.at(P()->tileCornerColor()), 1.0);
    }

    // Draw the fog
    if (m_foggy && S()->tileFogVisible()) {
        const GLfloat* fogColor = COLOR_STRINGS.at(P()->tileFogColor());
        GraphicUtilities::drawPolygon(m_tile->getFullPolygon(), COLOR_STRINGS.at(P()->tileFogColor()), P()->tileFogAlpha());
    }

    // TODO TODO TODO TODO : fix this
    // Draw the tile text, always padded to at least 3 characters (for distances)
    /*
    if (S()->tileTextVisible()) {
        glColor3fv(COLOR_STRINGS.at(P()->tileTextColor()));
        GraphicUtilities::drawText(
            Cartesian(Meters(m_tile->getX() * (P()->wallWidth() + P()->wallLength()) + P()->wallWidth()),
                      Meters(m_tile->getY() * (P()->wallWidth() + P()->wallLength()) + P()->wallWidth())),
            Meters(P()->wallLength()), Meters(P()->wallLength() / 3.0),
            std::string("   ").substr(0, (0 > 3 - (int) m_text.size() ? 0 : 3 - m_text.size())) + m_text);
    }
    */

    // TODO: Draw the shortest path graphic, which is usefule because this is
    // the metric by which mice that don't reach the center are ranked

}

void TileGraphic::setColor(const GLfloat* color) {
    m_color = color;
}

void TileGraphic::setText(const std::string& text) {
    m_text = text;
}

void TileGraphic::setFogginess(bool foggy) {
    m_foggy = foggy;
}

void TileGraphic::declareWall(Direction direction, bool isWall) {
    m_declaredWalls[direction] = isWall;
}

void TileGraphic::undeclareWall(Direction direction) {
    m_declaredWalls.erase(direction);
}

bool TileGraphic::wallDeclared(Direction direction) const {
    return m_declaredWalls.find(direction) != m_declaredWalls.end();
}

} // namespace sim

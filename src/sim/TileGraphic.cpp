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
    glColor3fv(baseColor);
    GraphicUtilities::drawPolygon(m_tile->getFullPolygon());

    // Either draw the true walls of the tile ...
    if (S()->wallTruthVisible()) {
        glColor3fv(COLOR_STRINGS.at(P()->tileWallColor()));
        for (Polygon polygon : m_tile->getActualWallPolygons()) {
            GraphicUtilities::drawPolygon(polygon);
        }
    }

    // ... or the algorithm's declared walls
    else {
        for (Direction direction : DIRECTIONS) {

            // If the wall was declared, use the wall color and tile base color ...
            if (m_declaredWalls.find(direction) != m_declaredWalls.end()) {
                if (m_declaredWalls.at(direction)) {
                    if (m_tile->isWall(direction)) {
                        glColor3fv(COLOR_STRINGS.at(P()->tileWallColor()));
                    }
                    else {
                        glColor3fv(COLOR_STRINGS.at(P()->tileIncorrectlyDeclaredWallColor()));
                    }
                }
                else {
                    if (m_tile->isWall(direction)) {
                        glColor3fv(COLOR_STRINGS.at(P()->tileIncorrectlyDeclaredNoWallColor()));
                    }
                    else {
                        glColor3fv(baseColor);
                    }
                }
            }

            // ... otherwise, use the undeclared walls colors
            else {
                if (m_tile->isWall(direction)) {
                    glColor3fv(COLOR_STRINGS.at(P()->tileUndeclaredWallColor()));
                }
                else {
                    glColor3fv(COLOR_STRINGS.at(P()->tileUndeclaredNoWallColor()));
                }
            }

            GraphicUtilities::drawPolygon(m_tile->getWallPolygon(direction));
        }
    }

    // Draw the corners of the tile
    glColor3fv(COLOR_STRINGS.at(P()->tileCornerColor()));
    for (Polygon polygon : m_tile->getCornerPolygons()) {
        GraphicUtilities::drawPolygon(polygon);
    }

    // Draw the fog
    if (m_foggy && S()->tileFogVisible()) {
        const GLfloat* fogColor = COLOR_STRINGS.at(P()->tileFogColor());
        const GLfloat fogColorWithAlpha[] = {fogColor[0], fogColor[1], fogColor[2], P()->tileFogAlpha()};
        glColor4fv(fogColorWithAlpha);
        GraphicUtilities::drawPolygon(m_tile->getFullPolygon());
    }

    // Draw the tile text, always padded to at least 3 characters (for distances)
    if (S()->tileTextVisible()) {
        glColor3fv(COLOR_STRINGS.at(P()->tileTextColor()));
        GraphicUtilities::drawText(
            Cartesian(Meters(m_tile->getX() * (P()->wallWidth() + P()->wallLength()) + P()->wallWidth()),
                      Meters(m_tile->getY() * (P()->wallWidth() + P()->wallLength()) + P()->wallWidth())),
            Meters(P()->wallLength()), Meters(P()->wallLength() / 3.0),
            std::string("   ").substr(0, (0 > 3 - (int) m_text.size() ? 0 : 3 - m_text.size())) + m_text);
    }

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

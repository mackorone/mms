#include "TileGraphic.h"

#include "Color.h"
#include "GraphicUtilities.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"

namespace sim {

TileGraphic::TileGraphic(const Tile* tile) : m_tile(tile), m_color(STRING_TO_COLOR.at(P()->tileBaseColor())),
        m_distance(0), m_foggy(true) {
}

bool TileGraphic::wallDeclared(Direction direction) const {
    return SimUtilities::mapContains(m_declaredWalls, direction);
}

void TileGraphic::setColor(Color color) {
    m_color = color;
    updateColor();
}

void TileGraphic::setDistance(int distance) {
    m_distance = distance;
    updateDistance();
}

void TileGraphic::setFogginess(bool foggy) {
    m_foggy = foggy;
    updateFog();
}

void TileGraphic::declareWall(Direction direction, bool isWall) {
    m_declaredWalls[direction] = isWall;
    updateWall(direction);
}

void TileGraphic::undeclareWall(Direction direction) {
    m_declaredWalls.erase(direction);
    updateWall(direction);
}

void TileGraphic::draw() const {

    // Draw the base of the tile
    GraphicUtilities::drawTileGraphicBase(m_tile->getX(), m_tile->getY(), m_tile->getFullPolygon(),
        S()->tileColorsVisible() ? m_color : STRING_TO_COLOR.at(P()->tileBaseColor()));

    // TODO: MACK - Draw the tile gridlines...
    // Ideally, we should only have to draw a few lines that span the entire maze, as opposed to many little lines...

    // Draw each of the walls of the tile
    for (Direction direction : DIRECTIONS) {

        // Get the wall color and alpha
        std::pair<Color, float> colorAndAlpha = deduceWallColorAndAlpha(direction);

        // Draw the polygon
        GraphicUtilities::drawTileGraphicWall(m_tile->getX(), m_tile->getY(), direction,
            m_tile->getWallPolygon(direction), colorAndAlpha.first, colorAndAlpha.second);
    }

    // Draw the corners of the tile
    std::vector<Polygon> cornerPolygons = m_tile->getCornerPolygons();
    for (int cornerNumber = 0; cornerNumber < cornerPolygons.size(); cornerNumber += 1) {
        GraphicUtilities::drawTileGraphicCorner(m_tile->getX(), m_tile->getY(), cornerNumber,
            cornerPolygons.at(cornerNumber), STRING_TO_COLOR.at(P()->tileCornerColor()));
    }

    // Draw the distance character textures
    for (int row = 0; row < 2; row += 1) {
        for (int col = 0; col < 4; col += 1) {
            // TODO: MACK - pass in the correct characters here
            GraphicUtilities::drawTileGraphicDistanceCharacter(m_tile->getX(), m_tile->getY(), row, col,
                getDistanceCharacterPolygon(row, col), ((col + row) % 2 == 0 ? 'a' : 'b'));
        }
    }

    // Draw the fog
    GraphicUtilities::drawTileGraphicFog(m_tile->getX(), m_tile->getY(), m_tile->getFullPolygon(),
        STRING_TO_COLOR.at(P()->tileFogColor()), m_foggy && S()->tileFogVisible() ? P()->tileFogAlpha() : 0.0);
}

void TileGraphic::updateColor() const {
    GraphicUtilities::updateTileGraphicBaseColor(m_tile->getX(), m_tile->getY(),
        S()->tileColorsVisible() ? m_color : STRING_TO_COLOR.at(P()->tileBaseColor()));
}

void TileGraphic::updateDistance() const {
    // TODO: MACK - change tile text to tile color, 
    // TODO: MACK - add a param for whether or not the actual distance values displayed
    // TODO: MACK - text color???
    for (int row = 0; row < 2; row += 1) {
        for (int col = 0; col < 4; col += 1) {
            /*
            GraphicUtilities::updateTileGraphicDistanceCharacter(m_tile->getX(), m_tile->getY(),
                row, col, S()->tileTextVisible() ? m_color : STRING_TO_COLOR.at(P()->tileBaseColor()));
            */
        }
    }
}

void TileGraphic::updateFog() const {
    GraphicUtilities::updateTileGraphicFog(m_tile->getX(), m_tile->getY(),
        m_foggy && S()->tileFogVisible() ? P()->tileFogAlpha() : 0.0);
}

void TileGraphic::updateWalls() const {
    for (Direction direction : DIRECTIONS) {
        updateWall(direction);
    }
}

void TileGraphic::updateWall(Direction direction) const {
    std::pair<Color, float> colorAndAlpha = deduceWallColorAndAlpha(direction);
    GraphicUtilities::updateTileGraphicWallColor(m_tile->getX(), m_tile->getY(), direction,
        colorAndAlpha.first, colorAndAlpha.second);
}

std::pair<Color, float> TileGraphic::deduceWallColorAndAlpha(Direction direction) const {

    // Declare the wall color and alpha, assign defaults
    Color wallColor = STRING_TO_COLOR.at(P()->tileWallColor());
    float wallAlpha = 1.0;

    // Either draw the true walls of the tile ...
    if (S()->wallTruthVisible()) {
        wallAlpha = m_tile->isWall(direction) ? 1.0 : 0.0;
    }

    // ... or the algorithm's (un)declared walls
    else {

        // If the wall was declared, use the wall color and tile base color ...
        if (wallDeclared(direction)) {
            if (m_declaredWalls.at(direction)) {
                // Correct declaration
                if (m_tile->isWall(direction)) {
                    wallColor = STRING_TO_COLOR.at(P()->tileWallColor());
                }
                // Incorrect declaration
                else {
                    wallColor = STRING_TO_COLOR.at(P()->tileIncorrectlyDeclaredWallColor());
                }
            }
            else {
                // Incorrect declaration
                if (m_tile->isWall(direction)) {
                    wallColor = STRING_TO_COLOR.at(P()->tileIncorrectlyDeclaredNoWallColor());
                }
                // Correct declaration
                else {
                    wallAlpha = 0.0;
                }
            }
        }

        // ... otherwise, use the undeclared walls colors
        else {
            if (m_tile->isWall(direction)) {
                wallColor = STRING_TO_COLOR.at(P()->tileUndeclaredWallColor());
            }
            else {
                wallColor = STRING_TO_COLOR.at(P()->tileUndeclaredNoWallColor());
            }
        }
    }

    return std::make_pair(wallColor, wallAlpha);
}

Polygon TileGraphic::getDistanceCharacterPolygon(int row, int col) const {

    //                  *---*-------------------*---*
    //                  |   |                   |   |
    //                  *---*-------------------Y---*
    //                  |   |    |    |    |    |   |
    //                  |   |    |    |    |    |   |
    //                  |   | 00 | 01 | 02 | 03 |   |
    //                  |   |____|____|____|____|   |
    //                  |   |    |    |    |    |   |
    //                  |   |    |    |    |    |   |
    //                  |   | 10 | 11 | 12 | 13 |   |
    //                  |   |    |    |    |    |   |
    //                  *---X-------------------*---*
    //                  |   |                   |   |
    //                  *---*-------------------*---*

    Cartesian X = m_tile->getInteriorPolygon().getVertices().at(0);
    Cartesian Y = m_tile->getInteriorPolygon().getVertices().at(2);
    Cartesian diagonal = Y - X;
    Meters characterWidth = diagonal.getX() / 4.0;
    Meters characterHeight = diagonal.getY() / 2.0;
    return Polygon({
        X + Cartesian(characterWidth *  col     , characterHeight *  row     ),
        X + Cartesian(characterWidth *  col     , characterHeight * (row + 1)),
        X + Cartesian(characterWidth * (col + 1), characterHeight * (row + 1)),
        X + Cartesian(characterWidth * (col + 1), characterHeight *  row     )});
}

} // namespace sim

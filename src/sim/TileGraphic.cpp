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

    // Note that the order in which we call insertIntoGraphicCpuBuffer
    // determines the order in which the polygons are drawn. Also note that the
    // *StartingIndex methods in GrahicsUtilities.h depend upon this order.

    // Draw the base of the tile
    GraphicUtilities::insertIntoGraphicCpuBuffer(
        m_tile->getFullPolygon(),
        S()->tileColorsVisible() ? m_color : STRING_TO_COLOR.at(P()->tileBaseColor()),
        1.0);

    // Draw each of the walls of the tile
    for (Direction direction : DIRECTIONS) {
        std::pair<Color, float> colorAndAlpha = deduceWallColorAndAlpha(direction);
        GraphicUtilities::insertIntoGraphicCpuBuffer(
            m_tile->getWallPolygon(direction),
            colorAndAlpha.first,
            colorAndAlpha.second);
    }

    // Draw the corners of the tile
    for (Polygon polygon : m_tile->getCornerPolygons()) {
        GraphicUtilities::insertIntoGraphicCpuBuffer(
            polygon,
            STRING_TO_COLOR.at(P()->tileCornerColor()),
            1.0);
    }

    // Draw the fog
    GraphicUtilities::insertIntoGraphicCpuBuffer(
        m_tile->getFullPolygon(),
        STRING_TO_COLOR.at(P()->tileFogColor()),
        m_foggy && S()->tileFogVisible() ? P()->tileFogAlpha() : 0.0);

    // TODO: MACK - Draw the tile gridlines...
    // Ideally, we should only have to draw a few lines that span the entire maze, as opposed to many little lines...

    // Draw the distance character textures
    static int numRows = 2; // TODO: MACK - Make this a constant somewhere
    static int numCols = 4; // TODO: MACK - Make this a constant somewhere
    for (int row = 0; row < numRows; row += 1) {
        for (int col = 0; col < numCols; col += 1) {
            GraphicUtilities::insertIntoTextureCpuBuffer(
                getTextPolygon(numRows, numCols, row, col),
                ' ');
        }
    }
}

void TileGraphic::updateColor() const {
    GraphicUtilities::updateTileGraphicBaseColor(m_tile->getX(), m_tile->getY(),
        S()->tileColorsVisible() ? m_color : STRING_TO_COLOR.at(P()->tileBaseColor()));
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

void TileGraphic::updateDistance() const {
    for (int row = 0; row < 2; row += 1) {
        for (int col = 0; col < 4; col += 1) {
            /*
            GraphicUtilities::updateTileGraphicDistanceCharacter(m_tile->getX(), m_tile->getY(),
                row, col, S()->tileTextVisible() ? m_color : STRING_TO_COLOR.at(P()->tileBaseColor()));
            */
        }
    }
}

Polygon TileGraphic::getTextPolygon(int numRows, int numCols, int row, int col) const {

    //                  *---*-------------------*---*
    //                  |   |                   |   |
    //                  *---*-------------------Y---*
    //                  |   |    |    |    |    |   |
    //                  |   |    |    |    |    |   |
    //                  |   | 10 | 11 | 12 | 13 |   |
    //                  |   |____|____|____|____|   |
    //                  |   |    |    |    |    |   |
    //                  |   |    |    |    |    |   |
    //                  |   | 00 | 01 | 02 | 03 |   |
    //                  |   |    |    |    |    |   |
    //                  *---X-------------------*---*
    //                  |   |                   |   |
    //                  *---*-------------------*---*

    Cartesian X = m_tile->getInteriorPolygon().getVertices().at(0);
    Cartesian Y = m_tile->getInteriorPolygon().getVertices().at(2);
    Cartesian diagonal = Y - X;
    Meters characterWidth = diagonal.getX() / static_cast<double>(numCols);
    Meters characterHeight = diagonal.getY() / static_cast<double>(numRows);
    return Polygon({
        X + Cartesian(characterWidth *  col     , characterHeight *  row     ),
        X + Cartesian(characterWidth *  col     , characterHeight * (row + 1)),
        X + Cartesian(characterWidth * (col + 1), characterHeight * (row + 1)),
        X + Cartesian(characterWidth * (col + 1), characterHeight *  row     )});
}

} // namespace sim

#include "TileGraphic.h"

#include "Color.h"
#include "CPMinMax.h"
#include "GraphicUtilities.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"

namespace sim {

TileGraphic::TileGraphic(const Tile* tile) : m_tile(tile), m_color(STRING_TO_COLOR.at(P()->tileBaseColor())),
        m_foggy(true) {
}

bool TileGraphic::wallDeclared(Direction direction) const {
    return SimUtilities::mapContains(m_declaredWalls, direction);
}

void TileGraphic::setColor(Color color) {
    m_color = color;
    updateColor();
}

void TileGraphic::declareWall(Direction direction, bool isWall) {
    m_declaredWalls[direction] = isWall;
    updateWall(direction);
}

void TileGraphic::undeclareWall(Direction direction) {
    m_declaredWalls.erase(direction);
    updateWall(direction);
}

void TileGraphic::setFogginess(bool foggy) {
    m_foggy = foggy;
    updateFog();
}

void TileGraphic::setText(const std::vector<std::string>& rowsOfText) {
    m_rowsOfText = rowsOfText;
    updateText();
    if (P()->setTileBaseColorWhenDistanceCorrect() && 0 < rowsOfText.size()
        && std::to_string(m_tile->getDistance()) == rowsOfText.at(0)) {
            setColor(STRING_TO_COLOR.at(P()->distanceCorrectTileBaseColor()));
    }
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


    // Insert all of the triangle texture objects into the buffer ...
    std::pair<int, int> maxRowsAndCols = GraphicUtilities::getTileGraphicTextMaxSize();
    for (int row = 0; row < maxRowsAndCols.first; row += 1) {
        for (int col = 0; col < maxRowsAndCols.second; col += 1) {
            GraphicUtilities::insertIntoTextureCpuBuffer();
        }
    }
    // ... and then populate those triangle texture objects with data
    updateText();
}

void TileGraphic::updateColor() const {
    GraphicUtilities::updateTileGraphicBaseColor(m_tile->getX(), m_tile->getY(),
        S()->tileColorsVisible() ? m_color : STRING_TO_COLOR.at(P()->tileBaseColor()));
}

void TileGraphic::updateWalls() const {
    for (Direction direction : DIRECTIONS) {
        updateWall(direction);
    }
}

void TileGraphic::updateFog() const {
    GraphicUtilities::updateTileGraphicFog(m_tile->getX(), m_tile->getY(),
        m_foggy && S()->tileFogVisible() ? P()->tileFogAlpha() : 0.0);
}

void TileGraphic::updateText() const {

    std::vector<std::string> rows = m_rowsOfText;
    if (S()->tileDistanceVisible()) {
        rows.insert(rows.begin(), (0 <= m_tile->getDistance() ? std::to_string(m_tile->getDistance()) : "inf"));
    }

    std::pair<int, int> maxRowsAndCols = GraphicUtilities::getTileGraphicTextMaxSize();
    for (int row = 0; row < maxRowsAndCols.first; row += 1) {
        for (int col = 0; col < maxRowsAndCols.second; col += 1) {
            GraphicUtilities::updateTileGraphicText(
                m_tile,
                std::min(static_cast<int>(rows.size()), maxRowsAndCols.first),
                std::min(
                    static_cast<int>((row < rows.size() ? rows.at(row).size() : 0)),
                    maxRowsAndCols.second),
                row,
                col,
                (S()->tileTextVisible() && row < rows.size() && col < rows.at(row).size() ? rows.at(row).at(col) : ' '));
        }
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

} // namespace sim

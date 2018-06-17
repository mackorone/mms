#include "TileGraphic.h"

#include <QPair>

#include "Assert.h"
#include "Color.h"
#include "ColorManager.h"
#include "FontImage.h"
#include "Param.h"

namespace mms {

TileGraphic::TileGraphic() :
    m_tile(nullptr),
    m_bufferInterface(nullptr),
    m_color(Color::BLACK) {
}

TileGraphic::TileGraphic(
    const Tile* tile,
    BufferInterface* bufferInterface) :
    m_tile(tile),
    m_bufferInterface(bufferInterface),
    m_color(ColorManager::get()->getTileBaseColor()) {
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
    m_declaredWalls.remove(direction);
    updateWall(direction);
}

void TileGraphic::setText(const QString& text) {
    m_text = text;
    updateText();
}

void TileGraphic::drawPolygons() const {

    // Note that the order in which we call insertIntoGraphicCpuBuffer
    // determines the order in which the polygons are drawn. Also note that the
    // *StartingIndex methods in GrahicsUtilities.h depend upon this order.

    // Draw the base of the tile
    m_bufferInterface->insertIntoGraphicCpuBuffer(
        m_tile->getFullPolygon(), m_color, 1.0);

    // Draw each of the walls of the tile
    for (Direction direction : DIRECTIONS()) {
        QPair<Color, float> colorAndAlpha = deduceWallColorAndAlpha(direction);
        m_bufferInterface->insertIntoGraphicCpuBuffer(
            m_tile->getWallPolygon(direction),
            colorAndAlpha.first,
            colorAndAlpha.second);
    }

    // Draw the corners of the tile
    for (Polygon polygon : m_tile->getCornerPolygons()) {
        m_bufferInterface->insertIntoGraphicCpuBuffer(
            polygon,
            ColorManager::get()->getTileCornerColor(),
            1.0);
    }
}

void TileGraphic::drawTextures() {
    // Insert all of the triangle texture objects into the buffer ...
    QPair<int, int> maxRowsAndCols =
        m_bufferInterface->getTileGraphicTextMaxSize();
    for (int row = 0; row < maxRowsAndCols.first; row += 1) {
        for (int col = 0; col < maxRowsAndCols.second; col += 1) {
            m_bufferInterface->insertIntoTextureCpuBuffer();
        }
    }
    // ... and then populate those triangle texture objects with data
    updateText();
}

void TileGraphic::updateColor() const {
    m_bufferInterface->updateTileGraphicBaseColor(
        m_tile->getX(),
        m_tile->getY(),
        m_color);
}

void TileGraphic::updateWalls() const {
    for (Direction direction : DIRECTIONS()) {
        updateWall(direction);
    }
}

void TileGraphic::updateText() const {

    // First, retrieve the maximum number of rows and cols of text allowed
    QPair<int, int> maxRowsAndCols =
        m_bufferInterface->getTileGraphicTextMaxSize();

    // Then, generate the rows of text that will be displayed
    QVector<QString> rowsOfText;

    // Split the text into rows
    QString remaining = m_text;
    while (!remaining.isEmpty() && rowsOfText.size() < maxRowsAndCols.first) {
        QString row = remaining.left(maxRowsAndCols.second);
        rowsOfText.append(row);
        remaining = remaining.mid(maxRowsAndCols.second);
    }

    // For all possible character positions, insert some character
    // (blank if necessary) into the tile text cpu buffer
    for (int row = 0; row < maxRowsAndCols.first; row += 1) {
        for (int col = 0; col < maxRowsAndCols.second; col += 1) {
            int numRows = std::min(
                static_cast<int>(rowsOfText.size()),
                maxRowsAndCols.first
            );
            int numCols = std::min(
                static_cast<int>(row < rowsOfText.size() ? rowsOfText.at(row).size() : 0),
                maxRowsAndCols.second
            );
            QChar c = ' ';
            if (
                row < rowsOfText.size() &&
                col < rowsOfText.at(row).size()
            ) {
                c = rowsOfText.at(row).at(col).toLatin1();
            }
            ASSERT_TR(FontImage::get()->positions().contains(c));
            m_bufferInterface->updateTileGraphicText(
                m_tile->getX(),
                m_tile->getY(),
                numRows,
                numCols,
                row,
                col,
                c
            );
        }
    }
}

void TileGraphic::updateWall(Direction direction) const {
    QPair<Color, float> colorAndAlpha = deduceWallColorAndAlpha(direction);
    m_bufferInterface->updateTileGraphicWallColor(
        m_tile->getX(),
        m_tile->getY(),
        direction,
        colorAndAlpha.first,
        colorAndAlpha.second
    );
}

QPair<Color, float> TileGraphic::deduceWallColorAndAlpha(Direction direction) const {

    // Declare the wall color and alpha, assign defaults
    Color wallColor = ColorManager::get()->getTileWallColor();
    float wallAlpha = 1.0;

    // If the wall was declared, use the wall color and tile base color ...
    if (m_declaredWalls.contains(direction)) {
        if (m_declaredWalls.value(direction)) {
            // Correct declaration
            if (m_tile->isWall(direction)) {
                wallColor = ColorManager::get()->getTileWallColor();
            }
            // Incorrect declaration
            else {
                wallColor = ColorManager::get()->getIncorrectlyDeclaredWallColor();
            }
        }
        else {
            // Incorrect declaration
            if (m_tile->isWall(direction)) {
                wallColor = ColorManager::get()->getIncorrectlyDeclaredNoWallColor();
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
            wallColor = ColorManager::get()->getUndeclaredWallColor();
        }
        else {
            wallColor = ColorManager::get()->getUndeclaredNoWallColor();
        }
    }
    
    // If the wall color is the same as the default tile base color,
    // we interpret that to mean that the walls should be transparent
    if (wallColor == ColorManager::get()->getTileBaseColor()) {
        wallAlpha = 0.0;
    }

    return {wallColor, wallAlpha};
}

} // namespace mms

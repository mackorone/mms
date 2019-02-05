#include "TileGraphic.h"

#include <QPair>

#include "AssertMacros.h"
#include "Color.h"
#include "ColorManager.h"
#include "FontImage.h"

namespace mms {

TileGraphic::TileGraphic() {
    ASSERT_NEVER_RUNS();
}

TileGraphic::TileGraphic(
    const Tile* tile,
    BufferInterface* bufferInterface) :
    m_tile(tile),
    m_bufferInterface(bufferInterface),
    m_color(ColorManager::getTileBaseColor()) {
}

void TileGraphic::setWall(Direction direction) {
    m_walls[direction] = true;
    updateWall(direction);
}

void TileGraphic::clearWall(Direction direction) {
    m_walls.remove(direction);
    updateWall(direction);
}

void TileGraphic::setColor(Color color) {
    m_color = color;
    updateColor();
}

void TileGraphic::clearColor() {
    m_color = ColorManager::getTileBaseColor();
    updateColor();
}

void TileGraphic::setText(const QString& text) {
    m_text = text;
    updateText();
}

void TileGraphic::clearText() {
    m_text = "";
    updateText();
}

void TileGraphic::drawPolygons() const {

    // Note that the order in which we call insertIntoGraphicCpuBuffer
    // determines the order in which the polygons are drawn. Also note that the
    // *StartingIndex methods in GrahicsUtilities.h depend upon this order.

    // Draw the base of the tile
    m_bufferInterface->insertIntoGraphicCpuBuffer(
        m_tile->getFullPolygon(), m_color, 255);

    // Draw each of the walls of the tile
    for (Direction direction : DIRECTIONS()) {
        m_bufferInterface->insertIntoGraphicCpuBuffer(
            m_tile->getWallPolygon(direction),
            ColorManager::getTileWallColor(),
            getWallAlpha(direction));
    }

    // Draw the corners of the tile
    for (Polygon polygon : m_tile->getCornerPolygons()) {
        m_bufferInterface->insertIntoGraphicCpuBuffer(
            polygon,
            ColorManager::getTileCornerColor(),
            255);
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

void TileGraphic::updateWall(Direction direction) const {
    m_bufferInterface->updateTileGraphicWallColor(
        m_tile->getX(),
        m_tile->getY(),
        direction,
        ColorManager::getTileWallColor(),
        getWallAlpha(direction)
    );
}


void TileGraphic::updateColor() const {
    m_bufferInterface->updateTileGraphicBaseColor(
        m_tile->getX(),
        m_tile->getY(),
        m_color);
}

void TileGraphic::updateText() const {

    // First, retrieve the maximum number of rows and cols of text allowed
    QPair<int, int> maxRowsAndCols =
        m_bufferInterface->getTileGraphicTextMaxSize();

    // Then, generate the rows of text that will be displayed
    QStringList rowsOfText;

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
                c = rowsOfText.at(row).at(col);
            }
            ASSERT_TR(FontImage::positions().contains(c));
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

unsigned char TileGraphic::getWallAlpha(Direction direction) const {
    if (m_walls.value(direction)) {
        return 255;
    }
    if (m_tile->isWall(direction)) {
        return 64;
    }
    return 0;
}

} 

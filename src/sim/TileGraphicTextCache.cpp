#include "TileGraphicTextCache.h"

#include "FontImage.h"

namespace mms {

void TileGraphicTextCache::init(
        const Distance& wallLength,
        const Distance& wallWidth,
        QPair<int, int> tileGraphicTextMaxSize,
        double borderFraction,
        TileTextAlignment tileTextAlignment) {

    m_wallLength = wallLength;
    m_wallWidth = wallWidth;
    m_tileGraphicTextMaxSize = tileGraphicTextMaxSize;
    m_tileGraphicTextPositions = buildPositionCache(borderFraction, tileTextAlignment);
}

QPair<int, int> TileGraphicTextCache::getTileGraphicTextMaxSize() const {
    return m_tileGraphicTextMaxSize;
}

QPair<double, double> TileGraphicTextCache::getFontImageCharacterPosition(QChar c) const {
    ASSERT_TR(FontImage::get()->positions().contains(c));
    return FontImage::get()->positions().value(c);
}

QPair<Coordinate, Coordinate> TileGraphicTextCache::getTileGraphicTextPosition(
        int x, int y, int numRows, int numCols, int row, int col) const {

    // Get the character position in the maze for the starting tile
    QPair<Coordinate, Coordinate> textPosition = m_tileGraphicTextPositions.value(
        {
            {numRows, numCols},
            {row, col}
        }
    );

    // Now get the character position in the maze for *this* tile
    Distance tileLength = m_wallLength + m_wallWidth;
    Coordinate offset = Coordinate::Cartesian(tileLength * x, tileLength * y);
    Coordinate LL = textPosition.first + offset;
    Coordinate UR = textPosition.second + offset;

    return {LL, UR};
}

QMap<
    QPair<QPair<int, int>, QPair<int, int>>,
    QPair<Coordinate, Coordinate>> TileGraphicTextCache::buildPositionCache(
        double borderFraction, TileTextAlignment tileTextAlignment) {

    // The tile graphic text could look like either of the following, depending
    // on the layout, border, and max size
    //
    //                    col
    //     *-*---------------------------*-*    *-*---------------------------*-*
    //     *-*--------------------------[B]*    *-*--------------------------[B]*
    //     | |                           | |    | |                           | |
    //     | |   *------------------[D]  | |    | |   *------------------[D]  | |
    //     | |   |    |    |    |    |   | |    | |   |                   |   | |
    //     | |   |    |    |    |    |   | |    | |   |                   |   | |
    //     | |   |    | 00 | 01 |    |   | |    | |   |----|----|----|----|   | |
    // row | |   |    |____|____|    |   | | or | |   |    |    |    |    |   | |
    //     | |   |    |    |    |    |   | |    | |   | 00 | 01 | 02 | 03 |   | |
    //     | |   |    |    |    |    |   | |    | |  [E]---|----|----|----|   | |
    //     | |   |    | 10 | 11 |    |   | |    | |   |                   |   | |
    //     | |   |    |    |    |    |   | |    | |   |                   |   | |
    //     | |  [C]--[E]-------------*   | |    | |  [C]------------------*   | |
    //     | |                           | |    | |                           | |
    //     *[A]--------------------------*-*    *[A]--------------------------*-*
    //     *-*---------------------------*-*    *-*---------------------------*-*

    QMap<
        QPair<QPair<int, int>, QPair<int, int>>,
        QPair<Coordinate, Coordinate>> positionCache;

    int maxRows = m_tileGraphicTextMaxSize.first;
    int maxCols = m_tileGraphicTextMaxSize.second;

    // First we get the unscaled diagonal
    Coordinate A = Coordinate::Cartesian(m_wallWidth / 2.0, m_wallWidth / 2.0);
    Coordinate B = A + Coordinate::Cartesian(m_wallLength, m_wallLength);
    Coordinate C = A + Coordinate::Cartesian(m_wallLength, m_wallLength) * borderFraction;
    Coordinate D = B - Coordinate::Cartesian(m_wallLength, m_wallLength) * borderFraction;
    Coordinate CD = D - C;

    // We assume that each character is twice as tall as it is wide, and we scale accordingly
    Distance characterWidth = CD.getX() / static_cast<double>(maxCols);
    Distance characterHeight = CD.getY() / static_cast<double>(maxRows);
    if (characterWidth * 2.0 < characterHeight) {
        characterHeight = characterWidth * 2.0;
    }
    else {
        characterWidth = characterHeight / 2.0;
    }

    // Now we get the scaled diagonal (note that we'll only shrink in at most one direction)
    Coordinate scalingOffset = Coordinate::Cartesian(
        (CD.getX() - characterWidth * maxCols) / 2.0,
        (CD.getY() - characterHeight * maxRows) / 2.0
    );
    Coordinate E = C + scalingOffset;

    // For all numbers of rows displayed
    for (int numRows = 0; numRows <= maxRows; numRows += 1) {

        // For all numbers of columns displayed
        for (int numCols = 0; numCols <= maxCols; numCols += 1) {

            // For each visible row and col
            for (int row = 0; row <= maxRows; row += 1) {
                for (int col = 0; col <= maxCols; col += 1) {

                    Coordinate LL = Coordinate::Cartesian(Distance::Meters(0), Distance::Meters(0));
                    Coordinate UR = Coordinate::Cartesian(Distance::Meters(0), Distance::Meters(0));

                    if (row < numRows && col < numCols) {

                        double rowOffset = 0.0;
                        if (CENTER_STAR_ALIGNMENTS().contains(tileTextAlignment)) {
                            rowOffset = static_cast<double>(maxRows - numRows) / 2.0;
                        }
                        else if (UPPER_STAR_ALIGNMENTS().contains(tileTextAlignment)) {
                            rowOffset = static_cast<double>(maxRows - numRows);
                        }

                        double colOffset = 0.0;
                        if (STAR_CENTER_ALIGNMENTS().contains(tileTextAlignment)) {
                            colOffset = static_cast<double>(maxCols - numCols) / 2.0;
                        }
                        else if (STAR_RIGHT_ALIGNMENTS().contains(tileTextAlignment)) {
                            colOffset = static_cast<double>(maxCols - numCols);
                        }

                        LL = Coordinate::Cartesian(
                            E.getX() + characterWidth * (col + colOffset),
                            E.getY() + characterHeight * ((numRows - row - 1) + rowOffset)
                        );
                        UR = Coordinate::Cartesian(
                            E.getX() + characterWidth * (col + colOffset + 1),
                            E.getY() + characterHeight * ((numRows - row - 1) + rowOffset + 1)
                        );
                    }

                    positionCache.insert(
                        {
                            // The number of rows/cols to be drawn
                            {numRows, numCols},
                            // The row and col of the current character
                            {row, col}
                        },
                        // The lower left and upper right texture coordinate
                        {LL, UR}
                    );
                }
            }
        }
    }

    return positionCache;
}

} // namespace mms

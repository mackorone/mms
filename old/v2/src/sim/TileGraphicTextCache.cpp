#include "TileGraphicTextCache.h"

namespace sim {

void TileGraphicTextCache::init(
        const Distance& wallLength,
        const Distance& wallWidth,
        std::pair<int, int> tileGraphicTextMaxSize,
        const std::map<char, std::pair<double, double>>& fontImageMap,
        double borderFraction,
        TileTextAlignment tileTextAlignment) {

    m_wallLength = wallLength;
    m_wallWidth = wallWidth;
    m_tileGraphicTextMaxSize = tileGraphicTextMaxSize;
    m_fontImageMap = fontImageMap;
    m_tileGraphicTextPositions = buildPositionCache(borderFraction, tileTextAlignment);
}

std::pair<int, int> TileGraphicTextCache::getTileGraphicTextMaxSize() const {
    return m_tileGraphicTextMaxSize;
}

std::pair<double, double> TileGraphicTextCache::getFontImageCharacterPosition(char c) const {
    SIM_ASSERT_TR(ContainerUtilities::mapContains(m_fontImageMap, c));
    return m_fontImageMap.at(c);
}

std::pair<Cartesian, Cartesian> TileGraphicTextCache::getTileGraphicTextPosition(
        int x, int y, int numRows, int numCols, int row, int col) const {

    // Get the character position in the maze for the starting tile
    std::pair<Cartesian, Cartesian> textPosition = m_tileGraphicTextPositions.at(
        std::make_pair(
            std::make_pair(numRows, numCols),
            std::make_pair(row, col)
        )
    );

    // Now get the character position in the maze for *this* tile
    Meters tileLength = m_wallLength + m_wallWidth;
    Cartesian offset = Cartesian(tileLength * x, tileLength * y);
    Cartesian LL = textPosition.first + offset;
    Cartesian UR = textPosition.second + offset;

    return std::make_pair(LL, UR);
}

std::map<
    std::pair<std::pair<int, int>, std::pair<int, int>>,
    std::pair<Cartesian, Cartesian>> TileGraphicTextCache::buildPositionCache(
        double borderFraction, TileTextAlignment tileTextAlignment) {

    // The tile graphic text could look like either of the following, depending
    // on the layout, boarder, and max size
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

    std::map<
        std::pair<std::pair<int, int>, std::pair<int, int>>,
        std::pair<Cartesian, Cartesian>> positionCache;

    int maxRows = m_tileGraphicTextMaxSize.first;
    int maxCols = m_tileGraphicTextMaxSize.second;

    // First we get the unscaled diagonal
    Cartesian A = Cartesian(m_wallWidth / 2.0, m_wallWidth / 2.0);
    Cartesian B = A + Cartesian(m_wallLength, m_wallLength);
    Cartesian C = A + Cartesian(m_wallLength, m_wallLength) * borderFraction;
    Cartesian D = B - Cartesian(m_wallLength, m_wallLength) * borderFraction;
    Cartesian CD = D - C;

    // We assume that each character is twice as tall as it is wide, and we scale accordingly
    Meters characterWidth = CD.getX() / static_cast<double>(maxCols);
    Meters characterHeight = CD.getY() / static_cast<double>(maxRows);
    if (characterWidth * 2.0 < characterHeight) {
        characterHeight = characterWidth * 2.0;
    }
    else {
        characterWidth = characterHeight / 2.0;
    }

    // Now we get the scaled diagonal (note that we'll only shrink in at most one direction)
    Cartesian scalingOffset = Cartesian(
        (CD.getX() - characterWidth * maxCols) / 2.0,
        (CD.getY() - characterHeight * maxRows) / 2.0
    );
    Cartesian E = C + scalingOffset;

    // For all numbers of rows displayed
    for (int numRows = 0; numRows <= maxRows; numRows += 1) {

        // For all numbers of columns displayed
        for (int numCols = 0; numCols <= maxCols; numCols += 1) {

            // For each visible row and col
            for (int row = 0; row <= maxRows; row += 1) {
                for (int col = 0; col <= maxCols; col += 1) {

                    Cartesian LL = Cartesian(Meters(0), Meters(0));
                    Cartesian UR = Cartesian(Meters(0), Meters(0));

                    if (row < numRows && col < numCols) {

                        double rowOffset = 0.0;
                        if (ContainerUtilities::setContains(CENTER_STAR_ALIGNMENTS, tileTextAlignment)) {
                            rowOffset = static_cast<double>(maxRows - numRows) / 2.0;
                        }
                        else if (ContainerUtilities::setContains(UPPER_STAR_ALIGNMENTS, tileTextAlignment)) {
                            rowOffset = static_cast<double>(maxRows - numRows);
                        }

                        double colOffset = 0.0;
                        if (ContainerUtilities::setContains(STAR_CENTER_ALIGNMENTS, tileTextAlignment)) {
                            colOffset = static_cast<double>(maxCols - numCols) / 2.0;
                        }
                        else if (ContainerUtilities::setContains(STAR_RIGHT_ALIGNMENTS, tileTextAlignment)) {
                            colOffset = static_cast<double>(maxCols - numCols);
                        }

                        LL = Cartesian(
                            Meters(E.getX() + characterWidth * (col + colOffset)),
                            Meters(E.getY() + characterHeight * ((numRows - row - 1) + rowOffset))
                        );
                        UR = Cartesian(
                            Meters(E.getX() + characterWidth * (col + colOffset + 1)),
                            Meters(E.getY() + characterHeight * ((numRows - row - 1) + rowOffset + 1))
                        );
                    }

                    positionCache.insert(
                        std::make_pair(
                            std::make_pair(
                                // The number of rows/cols to be drawn
                                std::make_pair(numRows, numCols),
                                // The row and col of the current character
                                std::make_pair(row, col)
                            ),
                            // The lower left and upper right texture coordinate
                            std::make_pair(LL, UR)
                        )
                    );
                }
            }
        }
    }

    return positionCache;
}

} // namespace sim

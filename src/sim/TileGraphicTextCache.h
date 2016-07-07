#pragma once

#include <QChar>
#include <QMap>
#include <QPair>

#include "TileTextAlignment.h"
#include "units/Cartesian.h"

namespace sim {

class TileGraphicTextCache {

public:
    
    // Initialize the cache
    void init(
        const Distance& wallLength,
        const Distance& wallWidth,
        QPair<int, int> tileGraphicTextMaxSize,
        const QMap<QChar, QPair<double, double>>& fontImageMap,
        double borderFraction,
        TileTextAlignment tiletextAlignment);

    // Returns the max number of rows and columns of tile graphic text
    QPair<int, int> getTileGraphicTextMaxSize() const;

    // Return a characters starting and ending position in the font image
    QPair<double, double> getFontImageCharacterPosition(QChar c) const;

    // Retrieve the LL and UR coordinates for a particular location
    QPair<Cartesian, Cartesian> getTileGraphicTextPosition(
        int x, int y, int numRows, int numCols, int row, int col) const;

private:

    // The length and width of maze walls, respectively
    Meters m_wallLength;
    Meters m_wallWidth;

    // The max rows and cols of text per tile
    QPair<int, int> m_tileGraphicTextMaxSize;

    // A map of char to the starting and ending horizontal positions (fraction
    // from 0.0 to 1.0) in the font image
    QMap<QChar, QPair<double, double>> m_fontImageMap;

    // A map of the number of rows/cols to be displayed and the current row/col
    // to the LL/UR text coordinates for the starting tile, namely tile (0, 0)
    QMap<
        QPair<QPair<int, int>, QPair<int, int>>,
        QPair<Cartesian, Cartesian>> m_tileGraphicTextPositions;

    // Just a helper method for building the text position cache
    QMap<
        QPair<QPair<int, int>, QPair<int, int>>,
        QPair<Cartesian, Cartesian>> buildPositionCache(
            double borderFraction,
            TileTextAlignment tiletextAlignment);
};

} // namespace sim

#pragma once

#include <map>

#include "TileTextAlignment.h"
#include "units/Cartesian.h"

namespace sim {

class TileGraphicTextCache {

public:
    
    // Initialize the cache
    void init(
        const Distance& wallLength,
        const Distance& wallWidth,
        std::pair<int, int> tileGraphicTextMaxSize,
        const std::map<char, std::pair<double, double>>& fontImageMap,
        double borderFraction,
        TileTextAlignment tiletextAlignment);

    // Returns the max number of rows and columns of tile graphic text
    std::pair<int, int> getTileGraphicTextMaxSize() const;

    // Return a characters starting and ending position in the font image
    std::pair<double, double> getFontImageCharacterPosition(char c) const;

    // Retrieve the LL and UR coordinates for a particular location
    std::pair<Cartesian, Cartesian> getTileGraphicTextPosition(
        int x, int y, int numRows, int numCols, int row, int col) const;

private:

    // The length and width of maze walls, respectively
    Meters m_wallLength;
    Meters m_wallWidth;

    // The max rows and cols of text per tile
    std::pair<int, int> m_tileGraphicTextMaxSize;

    // A map of char to the starting and ending horizontal positions (fraction
    // from 0.0 to 1.0) in the font image
    std::map<char, std::pair<double, double>> m_fontImageMap;

    // A map of the number of rows/cols to be displayed and the current row/col
    // to the LL/UR text coordinates for the starting tile, namely tile (0, 0)
    std::map<
        std::pair<std::pair<int, int>, std::pair<int, int>>,
        std::pair<Cartesian, Cartesian>> m_tileGraphicTextPositions;

    // Just a helper method for building the text position cache
    std::map<
        std::pair<std::pair<int, int>, std::pair<int, int>>,
        std::pair<Cartesian, Cartesian>> buildPositionCache(
            double borderFraction,
            TileTextAlignment tiletextAlignment);
};

} // namespace sim

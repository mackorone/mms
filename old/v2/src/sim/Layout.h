#pragma once

#include "LayoutType.h"

namespace sim {

class Layout {

public:

    // This class is not constructible
    Layout() = delete;

    // Return the positions of the full map and zoomed map, in pixels
    static std::pair<int, int> getFullMapPosition(
        int windowWidth,
        int windowHeight,
        int headerHeight,
        int windowBorderWidth,
        LayoutType layoutType);
    static std::pair<int, int> getZoomedMapPosition(
        int windowWidth,
        int windowHeight,
        int headerHeight,
        int windowBorderWidth,
        LayoutType layoutType);

    // Return the width and height of the full map and zoomed map, in pixels
    static std::pair<int, int> getFullMapSize(
        int windowWidth,
        int windowHeight,
        int headerHeight,
        int windowBorderWidth,
        LayoutType layoutType);
    static std::pair<int, int> getZoomedMapSize(
        int windowWidth,
        int windowHeight,
        int headerHeight,
        int windowBorderWidth,
        LayoutType layoutType);

};

} // namespace sim

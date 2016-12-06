#pragma once

#include <QPair>

#include "LayoutType.h"

namespace mms {

class Layout {

    // TODO: MACK - kill this

public:

    // This class is not constructible
    Layout() = delete;

    // Return the positions of the full map and zoomed map, in pixels
    static QPair<int, int> getFullMapPosition(
        int windowWidth,
        int windowHeight,
        int headerHeight,
        int windowBorderWidth,
        LayoutType layoutType);
    static QPair<int, int> getZoomedMapPosition(
        int windowWidth,
        int windowHeight,
        int headerHeight,
        int windowBorderWidth,
        LayoutType layoutType);

    // Return the width and height of the full map and zoomed map, in pixels
    static QPair<int, int> getFullMapSize(
        int windowWidth,
        int windowHeight,
        int headerHeight,
        int windowBorderWidth,
        LayoutType layoutType);
    static QPair<int, int> getZoomedMapSize(
        int windowWidth,
        int windowHeight,
        int headerHeight,
        int windowBorderWidth,
        LayoutType layoutType);

};

} // namespace mms

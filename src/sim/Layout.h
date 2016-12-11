#pragma once

#include <QPair>

#include "LayoutType.h"

namespace mms {

class Layout {

public:

    // This class is not constructible
    Layout() = delete;

    // The width, in pixels, of the full map's border
    static const int FULL_MAP_BORDER_WIDTH;

    // Return the positions of the full map and zoomed map, in pixels
    static QPair<int, int> getFullMapPosition();
    static QPair<int, int> getZoomedMapPosition();

    // Return the width and height of the full map and zoomed map, in pixels
    static QPair<int, int> getFullMapSize(int windowWidth, int windowHeight);
    static QPair<int, int> getZoomedMapSize(int windowWidth, int windowHeight);

};

} // namespace mms

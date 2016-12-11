#include "Layout.h"

#include <QPair>

namespace mms {

const int Layout::FULL_MAP_BORDER_WIDTH = 5;

QPair<int, int> Layout::getFullMapPosition() {
    return {FULL_MAP_BORDER_WIDTH, FULL_MAP_BORDER_WIDTH};
}

QPair<int, int> Layout::getZoomedMapPosition() {
    return {0, 0};
}

QPair<int, int> Layout::getFullMapSize(
        int windowWidth,
        int windowHeight,
        LayoutType layoutType) {

    if (layoutType == LayoutType::ZOOMED) {
        return {0, 0};
    }
    return {
        windowWidth - 2 * FULL_MAP_BORDER_WIDTH,
        windowHeight - 2 * FULL_MAP_BORDER_WIDTH
    };
}

QPair<int, int> Layout::getZoomedMapSize(
        int windowWidth,
        int windowHeight,
        LayoutType layoutType) {

    if (layoutType == LayoutType::FULL) {
        return {0, 0};
    }
    return {windowWidth, windowHeight};
}

} // namespace mms

#include "Layout.h"

namespace sim {

std::pair<int, int> Layout::getFullMapPosition(
        int windowWidth,
        int windowHeight,
        int headerHeight,
        int windowBorderWidth,
        LayoutType layoutType) {

    return std::make_pair(windowBorderWidth, windowBorderWidth);
}

std::pair<int, int> Layout::getZoomedMapPosition(
        int windowWidth,
        int windowHeight,
        int headerHeight,
        int windowBorderWidth,
        LayoutType layoutType) {

    if (layoutType == LayoutType::BOTH) {
        return std::make_pair((windowWidth + windowBorderWidth) / 2, windowBorderWidth);
    }
    return std::make_pair(windowBorderWidth, windowBorderWidth);
}

std::pair<int, int> Layout::getFullMapSize(
        int windowWidth,
        int windowHeight,
        int headerHeight,
        int windowBorderWidth,
        LayoutType layoutType) {

    int width = windowWidth - 2 * windowBorderWidth;
    int height = windowHeight - 2 * windowBorderWidth - headerHeight;

    if (layoutType == LayoutType::ZOOMED) {
        width = 0;
    }
    else if (layoutType == LayoutType::BOTH) {
        width = (width - windowBorderWidth) / 2;
    }
    return std::make_pair(width, height);
}

std::pair<int, int> Layout::getZoomedMapSize(
        int windowWidth,
        int windowHeight,
        int headerHeight,
        int windowBorderWidth,
        LayoutType layoutType) {

    int width = windowWidth - 2 * windowBorderWidth;
    int height = windowHeight - 2 * windowBorderWidth - headerHeight;

    if (layoutType == LayoutType::FULL) {
        width = 0;
    }
    else if (layoutType == LayoutType::BOTH) {
        width = (width - windowBorderWidth) / 2;
    }
    return std::make_pair(width, height);
}


} // namespace sim

#include "Layout.h"

#include <QPair>

namespace mms {

QPair<int, int> Layout::getFullMapPosition(
        int windowWidth,
        int windowHeight,
        int headerHeight,
        int windowBorderWidth,
        LayoutType layoutType) {

    return {windowBorderWidth, windowBorderWidth};
}

QPair<int, int> Layout::getZoomedMapPosition(
        int windowWidth,
        int windowHeight,
        int headerHeight,
        int windowBorderWidth,
        LayoutType layoutType) {

    if (layoutType == LayoutType::BOTH) {
        return {(windowWidth + windowBorderWidth) / 2, windowBorderWidth};
    }
    return {windowBorderWidth, windowBorderWidth};
}

QPair<int, int> Layout::getFullMapSize(
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
    return {width, height};
}

QPair<int, int> Layout::getZoomedMapSize(
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
    return {width, height};
}


} // namespace mms

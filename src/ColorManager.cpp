#include "ColorManager.h"

#include "Assert.h"

namespace mms {

Color ColorManager::getTileBaseColor() {
    return Color::BLACK;
}

Color ColorManager::getTileWallColor() {
    return Color::RED;
}

Color ColorManager::getTileCornerColor() {
    return Color::GRAY;
}

Color ColorManager::getMouseBodyColor() {
    return Color::BLUE;
}

Color ColorManager::getMouseWheelColor() {
    return Color::GREEN;
}

} 

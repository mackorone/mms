#pragma once

#include "Color.h"

namespace mms {

class ColorManager {

public:

    ColorManager() = delete;
    static Color getTileBaseColor();
    static Color getTileWallColor();
    static Color getTileCornerColor();
    static Color getMouseBodyColor();
    static Color getMouseWheelColor();

};

} 

#pragma once

#include <iostream>
#include <stdlib.h>
#include <vector>

#include "../lib/pugixml.hpp"

namespace sim {

class MouseParser {

public:
    MouseParser();
    // TODO: SOM
    // TODO: Get Left Wheel Width and Radius
    // TODO: Get Right Wheel Width and Radius
    // TODO: Get Body Polygon (encodes startin position info)

private:
    pugi::xml_document doc;

};

} // namespace sim

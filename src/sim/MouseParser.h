#pragma once

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "units/Cartesian.h"
#include "units/Meters.h"
#include "../lib/pugixml.hpp"

namespace sim {

class MouseParser {

public:
    MouseParser(const std::string& filePath);
    // TODO: Make these parameters enums instead of string for increased type safety
    Meters getWheelMeas(const std::string& wheel, const std::string& meas);
    Cartesian getWheelPosition(const std::string& wheel);

    std::vector<Cartesian> getBody();    
    float floatConvert(const char* numValue);
    
private:
    pugi::xml_document doc;
};

} // namespace sim

#pragma once

#include <Cartesian.h>
#include <Meters.h>
#include <pugixml.hpp>
#include <string>
#include <vector>

#include "Polygon.h"
#include "Sensor.h"
#include "Wheel.h"

namespace sim {

class MouseParser {

public:
    MouseParser(const std::string& filePath);
    Polygon getBody();    
    Wheel getWheel(WheelSide side);
    std::vector<Sensor> getSensors();
    
private:
    pugi::xml_document m_doc;
};

} // namespace sim

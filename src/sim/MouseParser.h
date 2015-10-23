#pragma once

#include <pugixml/pugixml.hpp>
#include <string>
#include <vector>

#include "Polygon.h"
#include "Sensor.h"
#include "units/Cartesian.h"
#include "units/Meters.h"
#include "Wheel.h"

namespace sim {

class MouseParser {

public:
    MouseParser(const std::string& filePath);
    Polygon getBody();
    std::map<std::string, Wheel> getWheels();
    std::map<std::string, Sensor> getSensors();

private:
    pugi::xml_document m_doc;
};

} // namespace sim

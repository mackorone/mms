#pragma once

#include "units/Cartesian.h"
#include "units/Meters.h"
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
    Wheel getLeftWheel();
    Wheel getRightWheel();
    std::map<std::string, Sensor> getSensors();

private:
    pugi::xml_document m_doc;

    Wheel getWheel(bool left);
};

} // namespace sim

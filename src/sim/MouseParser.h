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
    Radians getForwardDirection();
    Cartesian getCenterOfMass();
    Polygon getBody(Cartesian translationToAlignCenters);
    std::map<std::string, Wheel> getWheels(Cartesian translationToAlignCenters);
    std::map<std::string, Sensor> getSensors(Cartesian translationToAlignCenters);

private:
    pugi::xml_document m_doc;
};

} // namespace sim

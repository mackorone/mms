#pragma once

#include <iostream>
#include <stdlib.h>
#include <vector>

#include "../lib/pugixml.hpp"

namespace sim {

class MouseParser {

public:
    MouseParser();
    float getHeight();
    float getTorque();
    float getDiameter();
	float floatConvert(const char* numValue);
    std::vector<std::vector<float>> getWheels();
    std::vector<std::vector<float>> getSensors();
    std::vector<std::pair<float, float>> getShape();

private:
    pugi::xml_document doc;

};

} // namespace sim

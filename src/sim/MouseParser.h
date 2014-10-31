#pragma once
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "./lib/pugixml.hpp"

namespace sim {

class MouseParser {

pugi::xml_document doc;

public:

MouseParser();

std::vector<std::pair<float, float> > getShape();

float getHeight();

std::vector<std::vector<float> > getWheels();

float getTorque();

float getDiameter();

std::vector<std::vector<float> > getSensors();
};

}

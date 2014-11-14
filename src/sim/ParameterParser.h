#pragma once

#include <string>
#include <iostream>
#include "../lib/pugixml.hpp"

namespace sim {

class parameterParser {

	public:
		parameterParser();
		int getValue(char* name);
		std::string getMazeFile();

    private:
		pugi::xml_document doc;
};
}

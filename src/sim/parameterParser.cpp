#include "ParameterParser.h"

namespace sim {

parameterParser::parameterParser() {
	pugi::xml_parse_result result=doc.load_file("Parameters.xml");
	if (!result){
		std::cout << "Error" << std::endl;
		std::cout << "Description: " << result.description() << std::endl;
	}
}

int parameterParser::getValue(char* name) {
	return static_cast<int>(atof(doc.child("Parameters").child(name).child_value()));
}

std::string parameterParser::getMazeFile() {
	return static_cast<std::string>(doc.child("Parameters").child("MAZE_FILE").child_value()); //Casts from char* to string 
}

}

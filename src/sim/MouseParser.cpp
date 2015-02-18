#include "MouseParser.h"

namespace sim {

// TODO: SOM

MouseParser::MouseParser(const std::string& filePath) {
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());
    if (!result) {
		std::cout << "Error" << std::endl;
		std::cout << "description: " << result.description() << std::endl;
    }
}

Meters MouseParser::getWheelMeas(const std::string& wheel, const std::string& meas) {
    pugi::xml_node widthNode = doc.child(wheel.c_str()).child(meas.c_str());
    float width = floatConvert(widthNode.child_value());
    return Meters(width); 
} 

Cartesian MouseParser::getWheelPosition(const std::string& wheel) {
    pugi::xml_node posNode = doc.child(wheel.c_str()).child("position");
    float xValue = floatConvert(posNode.child("x").child_value());
    float yValue = floatConvert(posNode.child("y").child_value()); 
    return Cartesian(xValue, yValue);
}

std::vector<Cartesian> MouseParser::getBody() {
    pugi::xml_node bodyNode = doc.child("MouseBody");
    std::vector<Cartesian> points;
    float x, y;
    for(pugi::xml_node_iterator it = bodyNode.begin(); it != bodyNode.end(); ++it){
        if(*(it->name()) == 'x') {
            x = floatConvert(it->child_value());
        }
        else if(*(it->name()) == 'y') {
            y = floatConvert(it->child_value());
            std::cout << x << ' ' << y << std::endl;
            points.push_back(Cartesian(x, y));
        }
    }
    return points;
}

float MouseParser::floatConvert(const char* numValue){
        return static_cast<float>(atof(numValue));
}
} // namespace sim

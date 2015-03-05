#include "MouseParser.h"

#include "SimUtilities.h"

namespace sim {

MouseParser::MouseParser(const std::string& filePath) {
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());
    if (!result) {
		std::cout << "Error" << std::endl;
		std::cout << "description: " << result.description() << std::endl;
    }
}

Meters MouseParser::getWheelMeas(const std::string& wheel, const std::string& meas) {
    pugi::xml_node widthNode = doc.child(wheel.c_str()).child(meas.c_str());
    float width = strToFloat(widthNode.child_value());
    return Meters(width); 
} 

Cartesian MouseParser::getWheelPosition(const std::string& wheel) {
    pugi::xml_node posNode = doc.child(wheel.c_str()).child("position");
    float xValue = strToFloat(posNode.child("x").child_value());
    float yValue = strToFloat(posNode.child("y").child_value()); 
    return Cartesian(Meters(xValue), Meters(yValue));
}

std::vector<Cartesian> MouseParser::getBody() {
    pugi::xml_node bodyNode = doc.child("MouseBody");
    std::vector<Cartesian> points;
    float x, y;
    for(auto it = bodyNode.begin(); it != bodyNode.end(); ++it){
        if(*(it->name()) == 'x') {
            x = strToFloat(it->child_value());
        }
        else if(*(it->name()) == 'y') {
            y = strToFloat(it->child_value());
            points.push_back(Cartesian(Meters(x), Meters(y)));
        }
    }
    return points;
}

} // namespace sim

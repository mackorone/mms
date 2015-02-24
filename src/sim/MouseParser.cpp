#include "MouseParser.h"

#include "Assert.h"
#include "SimUtilities.h"

namespace sim {

MouseParser::MouseParser(const std::string& filePath) {
    // Open the document
    pugi::xml_parse_result result = m_doc.load_file(filePath.c_str());
    if (!result) {
        print("Unable to read mouse parameters in \"" + filePath + "\": " + result.description());
    }
    // TODO: How to handle the failing case???
    ASSERT(result);
}

Polygon MouseParser::getBody() {
    std::vector<Cartesian> points;
    pugi::xml_node body = m_doc.child("Body");
    for (auto it = body.begin(); it != body.end(); ++it) {
        pugi::xml_node point = *it;
        float x = strToFloat(point.child("X").child_value());
        print("" + x);
        for (auto it2 = point.begin(); it2 != point.end(); ++it2) {
            print (it2->name());
        }
        
        /*
        if (*(it->name()) == 'x') {
            x = strToFloat(it->child_value());
        }
        else if (*(it->name()) == 'y') {
            y = strToFloat(it->child_value());
            points.push_back(Cartesian(Meters(x), Meters(y)));
        }
        */
    }
    return Polygon(points);
}

Wheel MouseParser::getWheel(WheelSide side) {
    // TODO
    return Wheel();    
}

std::vector<Sensor> MouseParser::getSensors() {
    std::vector<Sensor> sensors;
    // TODO
    return sensors;
}

/*
Meters MouseParser::getWheelMeas(const std::string& wheel, const std::string& meas) {
    pugi::xml_node widthNode = m_doc.child(wheel.c_str()).child(meas.c_str());
    float width = strToFloat(widthNode.child_value());
    return Meters(width); 
} 

Cartesian MouseParser::getWheelPosition(const std::string& wheel) {
    pugi::xml_node posNode = m_doc.child(wheel.c_str()).child("position");
    float xValue = strToFloat(posNode.child("x").child_value());
    float yValue = strToFloat(posNode.child("y").child_value()); 
    return Cartesian(Meters(xValue), Meters(yValue));
}

std::vector<Cartesian> MouseParser::getBody() {
}
*/

} // namespace sim

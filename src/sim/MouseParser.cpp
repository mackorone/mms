#include "MouseParser.h"

#include "Assert.h"
#include "SimUtilities.h"

namespace sim {

// TODO: Make these strings variables...

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
        pugi::xml_node p = *it;
        float x = strToFloat(p.child("X").child_value());
        float y = strToFloat(p.child("Y").child_value());
        points.push_back(Cartesian(Meters(x), Meters(y)));
    }
    return Polygon(points);
}

Wheel MouseParser::getWheel(WheelSide side) {
    pugi::xml_node wheel = m_doc.child("Wheels").child((side == LEFT ? "Left" : "Right"));
    float radius = strToFloat(wheel.child("Radius").child_value());
    float width = strToFloat(wheel.child("Width").child_value());
    float x = strToFloat(wheel.child("Position").child("X").child_value());
    float y = strToFloat(wheel.child("Position").child("Y").child_value());
    return Wheel(Meters(radius), Meters(width), Cartesian(Meters(x), Meters(y)));    
}

std::vector<Sensor> MouseParser::getSensors() {
    std::vector<Sensor> sensors;
    // TODO
    return sensors;
}

} // namespace sim

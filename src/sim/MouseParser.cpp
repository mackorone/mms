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
        pugi::xml_node p = *it;
        float x = strToFloat(p.child("X").child_value());
        float y = strToFloat(p.child("Y").child_value());
        points.push_back(Cartesian(Meters(x), Meters(y)));
    }
    return Polygon(points);
}

Wheel MouseParser::getLeftWheel() {
    return getWheel(true);
}

Wheel MouseParser::getRightWheel() {
    return getWheel(false);
}

Wheel MouseParser::getWheel(bool left) {
    // TODO: Handle the failing case
    pugi::xml_node wheel = m_doc.child((left ? "LeftWheel" : "RightWheel"));
    float radius = strToFloat(wheel.child("Radius").child_value());
    float width = strToFloat(wheel.child("Width").child_value());
    float x = strToFloat(wheel.child("Position").child("X").child_value());
    float y = strToFloat(wheel.child("Position").child("Y").child_value());
    return Wheel(Meters(radius), Meters(width), Cartesian(Meters(x), Meters(y)));
}

std::map<std::string, Sensor> MouseParser::getSensors() {
    // TODO: Handle the failing case
    std::map<std::string, Sensor> sensors;
    for (pugi::xml_node sensor : m_doc.children("Sensor")) {
        std::string name(sensor.child("Name").child_value());
        // TODO: Check for duplicate name
        float x = strToFloat(sensor.child("Position").child("X").child_value());
        float y = strToFloat(sensor.child("Position").child("Y").child_value());
        float radius = strToFloat(sensor.child("Radius").child_value());
        float rotation = strToFloat(sensor.child("Rotation").child_value());
        float range = strToFloat(sensor.child("Range").child_value());
        float halfWidth = strToFloat(sensor.child("HalfWidth").child_value());
        float readTime = strToFloat(sensor.child("ReadTime").child_value());
        sensors.insert(std::make_pair(name,
            Sensor(Cartesian(Meters(x), Meters(y)), Meters(radius), Degrees(rotation), Meters(range), Degrees(halfWidth),
                   Seconds(readTime))));
    }
    return sensors;
}

} // namespace sim

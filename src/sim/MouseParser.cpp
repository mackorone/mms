#include "MouseParser.h"

#include "Assert.h"
#include "Logging.h"
#include "SimUtilities.h"

namespace sim {

MouseParser::MouseParser(const std::string& filePath) {
    // Open the document
    pugi::xml_parse_result result = m_doc.load_file(filePath.c_str());
    // TODO: Check the directories, etc. Give better error message to users
    if (!result) {
        L()->warn("Unable to read mouse parameters in \"%v\": %v", filePath, result.description());
        SimUtilities::quit(); // TODO: MACK - shouldn't quit here... allow other messages to print out
    }
    
    // TODO: MACK - use an explicit intialization step here so we can return false and then print out a more meaningful error message
}

Polygon MouseParser::getBody() {
    // TODO: Handle the failing case
    std::vector<Cartesian> points;
    pugi::xml_node body = m_doc.child("Body");
    for (auto it = body.begin(); it != body.end(); ++it) {
        pugi::xml_node p = *it;
        double x = SimUtilities::strToDouble(p.child("X").child_value());
        double y = SimUtilities::strToDouble(p.child("Y").child_value());
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
    double radius = SimUtilities::strToDouble(wheel.child("Radius").child_value());
    double width = SimUtilities::strToDouble(wheel.child("Width").child_value());
    double x = SimUtilities::strToDouble(wheel.child("Position").child("X").child_value());
    double y = SimUtilities::strToDouble(wheel.child("Position").child("Y").child_value());
    return Wheel(Meters(radius), Meters(width), Cartesian(Meters(x), Meters(y)));
}

std::map<std::string, Sensor> MouseParser::getSensors() {
    // TODO: Handle the failing case
    std::map<std::string, Sensor> sensors;
    for (pugi::xml_node sensor : m_doc.children("Sensor")) {
        std::string name(sensor.child("Name").child_value());
        // TODO: Check for duplicate name
        double x = SimUtilities::strToDouble(sensor.child("Position").child("X").child_value());
        double y = SimUtilities::strToDouble(sensor.child("Position").child("Y").child_value());
        double radius = SimUtilities::strToDouble(sensor.child("Radius").child_value());
        double rotation = SimUtilities::strToDouble(sensor.child("Rotation").child_value());
        double range = SimUtilities::strToDouble(sensor.child("Range").child_value());
        double halfWidth = SimUtilities::strToDouble(sensor.child("HalfWidth").child_value());
        double readDuration = SimUtilities::strToDouble(sensor.child("ReadDuration").child_value());
        sensors.insert(std::make_pair(name,
            Sensor(Cartesian(Meters(x), Meters(y)), Meters(radius), Degrees(rotation), Meters(range), Degrees(halfWidth),
                   Seconds(readDuration))));
    }
    return sensors;
}

} // namespace sim

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
        L()->error("Unable to read mouse parameters in \"%v\": %v", filePath, result.description());
        SimUtilities::quit(); // TODO: MACK - shouldn't quit here... allow other messages to print out
    }
    // TODO: MACK - use an explicit intialization step here so we can return false and then print out a more meaningful error message
}

Radians MouseParser::getForwardDirection() {
    // TODO: Handle the failing case
    double degrees = SimUtilities::strToDouble(
        m_doc.child("Forward-Direction").child_value());
    return Radians(Degrees(degrees));
}

Cartesian MouseParser::getCenterOfMass() {
    // TODO: Handle the failing case
    std::vector<Cartesian> vertices;
    pugi::xml_node center = m_doc.child("Center-of-Mass");
    double x = SimUtilities::strToDouble(center.child("X").child_value());
    double y = SimUtilities::strToDouble(center.child("Y").child_value());
    return Cartesian(Meters(x), Meters(y));
}

Polygon MouseParser::getBody(Cartesian translationToAlignCenters) {
    // TODO: Handle the failing case
    std::vector<Cartesian> vertices;
    pugi::xml_node body = m_doc.child("Body");
    for (auto it = body.begin(); it != body.end(); ++it) {
        pugi::xml_node p = *it;
        double x = SimUtilities::strToDouble(p.child("X").child_value());
        double y = SimUtilities::strToDouble(p.child("Y").child_value());
        vertices.push_back(Cartesian(Meters(x), Meters(y)) + translationToAlignCenters);
    }
    return Polygon(vertices);
}

std::map<std::string, Wheel> MouseParser::getWheels(Cartesian translationToAlignCenters) {
    // TODO: Handle the failing case
    std::map<std::string, Wheel> wheels;
    for (pugi::xml_node wheel : m_doc.children("Wheel")) {
        // TODO: Check for duplicate name
        std::string name(wheel.child("Name").child_value());
        double diameter = SimUtilities::strToDouble(wheel.child("Diameter").child_value());
        double width = SimUtilities::strToDouble(wheel.child("Width").child_value());
        double x = SimUtilities::strToDouble(wheel.child("Position").child("X").child_value());
        double y = SimUtilities::strToDouble(wheel.child("Position").child("Y").child_value());
        double direction = SimUtilities::strToDouble(wheel.child("Direction").child_value());
        wheels.insert(std::make_pair(name,
            Wheel(Meters(diameter), Meters(width),
                Cartesian(Meters(x), Meters(y)) + translationToAlignCenters, Degrees(direction))));
    }
    return wheels;
}

std::map<std::string, Sensor> MouseParser::getSensors(Cartesian translationToAlignCenters) {
    // TODO: Handle the failing case
    std::map<std::string, Sensor> sensors;
    for (pugi::xml_node sensor : m_doc.children("Sensor")) {
        // TODO: Check for duplicate name
        std::string name(sensor.child("Name").child_value());
        double radius = SimUtilities::strToDouble(sensor.child("Radius").child_value());
        double range = SimUtilities::strToDouble(sensor.child("Range").child_value());
        double halfWidth = SimUtilities::strToDouble(sensor.child("HalfWidth").child_value());
        double readDuration = SimUtilities::strToDouble(sensor.child("ReadDuration").child_value());
        double x = SimUtilities::strToDouble(sensor.child("Position").child("X").child_value());
        double y = SimUtilities::strToDouble(sensor.child("Position").child("Y").child_value());
        double direction = SimUtilities::strToDouble(sensor.child("Direction").child_value());
        sensors.insert(std::make_pair(name,
            Sensor(Meters(radius), Meters(range), Degrees(halfWidth), Seconds(readDuration),
                Cartesian(Meters(x), Meters(y)) + translationToAlignCenters, Degrees(direction))));
    }
    return sensors;
}

} // namespace sim

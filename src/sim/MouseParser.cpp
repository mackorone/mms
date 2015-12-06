#include "MouseParser.h"

#include "Assert.h"
#include "EncoderType.h"
#include "GeometryUtilities.h"
#include "Logging.h"
#include "SimUtilities.h"
#include "units/RevolutionsPerMinute.h"

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

Polygon MouseParser::getBody(
        const Cartesian& initialTranslation, const Radians& initialRotation) {
    Cartesian alignmentTranslation = initialTranslation - getCenterOfMass();
    Radians alignmentRotation = initialRotation - getForwardDirection();
    // TODO: Handle the failing case
    std::vector<Cartesian> vertices;
    pugi::xml_node body = m_doc.child("Body");
    for (auto it = body.begin(); it != body.end(); ++it) {
        pugi::xml_node p = *it;
        double x = SimUtilities::strToDouble(p.child("X").child_value());
        double y = SimUtilities::strToDouble(p.child("Y").child_value());
        vertices.push_back(
            alignVertex(
                Cartesian(Meters(x), Meters(y)),
                alignmentTranslation,
                alignmentRotation,
                initialTranslation));
    }
    return Polygon(vertices);
}

std::map<std::string, Wheel> MouseParser::getWheels(
        const Cartesian& initialTranslation, const Radians& initialRotation) {
    Cartesian alignmentTranslation = initialTranslation - getCenterOfMass();
    Radians alignmentRotation = initialRotation - getForwardDirection();
    // TODO: Handle the failing case (strToDouble will fail)
    std::map<std::string, Wheel> wheels;
    for (pugi::xml_node wheel : m_doc.children("Wheel")) {
        // TODO: Check for duplicate name
        std::string name(wheel.child("Name").child_value());
        double diameter = SimUtilities::strToDouble(wheel.child("Diameter").child_value());
        double width = SimUtilities::strToDouble(wheel.child("Width").child_value());
        double x = SimUtilities::strToDouble(wheel.child("Position").child("X").child_value());
        double y = SimUtilities::strToDouble(wheel.child("Position").child("Y").child_value());
        double direction = SimUtilities::strToDouble(wheel.child("Direction").child_value());
        double maxAngularVelocityMagnitude = SimUtilities::strToDouble(wheel.child("Max-Speed").child_value());
        std::string encoderTypeString = wheel.child("Encoder-Type").child_value();
        if (!SimUtilities::mapContains(STRING_TO_ENCODER_TYPE, encoderTypeString)) {
            L()->error(
                "The encoder type \"%v\" is not valid. The only valid encoder"
                " types are \"%v\" and \"%v\".",
                encoderTypeString, 
                ENCODER_TYPE_TO_STRING.at(EncoderType::ABSOLUTE),
                ENCODER_TYPE_TO_STRING.at(EncoderType::RELATIVE));
            // TODO: MACK - don't quit here, just return false???
            SimUtilities::quit();
        }
        EncoderType encoderType = STRING_TO_ENCODER_TYPE.at(encoderTypeString);
        double encoderTicksPerRevolution = SimUtilities::strToDouble(wheel.child("Encoder-Ticks-Per-Revolution").child_value());
        wheels.insert(
            std::make_pair(
                name,
                Wheel(
                    Meters(diameter),
                    Meters(width),
                    alignVertex(
                        Cartesian(Meters(x), Meters(y)),
                        alignmentTranslation,
                        alignmentRotation,
                        initialTranslation),
                    Degrees(direction) + alignmentRotation,
                    RevolutionsPerMinute(maxAngularVelocityMagnitude),
                    encoderType, 
                    encoderTicksPerRevolution)));
    }
    return wheels;
}

std::map<std::string, Sensor> MouseParser::getSensors(
        const Cartesian& initialTranslation, const Radians& initialRotation) {
    Cartesian alignmentTranslation = initialTranslation - getCenterOfMass();
    Radians alignmentRotation = initialRotation - getForwardDirection();
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
        sensors.insert(
            std::make_pair(
                name,
                Sensor(
                    Meters(radius),
                    Meters(range), 
                    Degrees(halfWidth),
                    Seconds(readDuration),
                    alignVertex(
                        Cartesian(Meters(x), Meters(y)),
                        alignmentTranslation,
                        alignmentRotation,
                        initialTranslation),
                    Degrees(direction) + alignmentRotation)));
    }
    return sensors;
}

Cartesian MouseParser::alignVertex(const Cartesian& vertex, const Cartesian& alignmentTranslation,
        const Radians& alignmentRotation, const Cartesian& rotationPoint) {
    Cartesian translated = GeometryUtilities::translateVertex(vertex, alignmentTranslation);
    return GeometryUtilities::rotateVertexAroundPoint(translated, alignmentRotation, rotationPoint);
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

} // namespace sim

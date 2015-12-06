#include "MouseParser.h"

#include "Assert.h"
#include "EncoderType.h"
#include "GeometryUtilities.h"
#include "Logging.h"
#include "SimUtilities.h"
#include "units/RevolutionsPerMinute.h"

namespace sim {

const Polygon MouseParser::NULL_POLYGON = Polygon({
    Cartesian(Meters(0), Meters(0)),
    Cartesian(Meters(0), Meters(0)),
    Cartesian(Meters(0), Meters(0)),
});
const std::string MouseParser::FORWARD_DIRECTION_TAG = "Forward-Direction";
const std::string MouseParser::CENTER_OF_MASS_TAG = "Center-Of-Mass";
const std::string MouseParser::BODY_TAG = "Body";
const std::string MouseParser::VERTEX_TAG = "Vertex";
const std::string MouseParser::X_TAG = "X";
const std::string MouseParser::Y_TAG = "Y";
const std::string MouseParser::WHEEL_TAG = "Wheel";
const std::string MouseParser::NAME_TAG = "Name";
const std::string MouseParser::DIAMETER_TAG = "Diameter";
const std::string MouseParser::WIDTH_TAG = "Width";
const std::string MouseParser::POSITION_TAG = "Position";
const std::string MouseParser::DIRECTION_TAG = "Direction";
const std::string MouseParser::MAX_SPEED_TAG = "Max-Speed";
const std::string MouseParser::ENCODER_TYPE_TAG = "Encoder-Type";
const std::string MouseParser::ENCODER_TICKS_PER_REVOLUTION_TAG = "Encoder-Ticks-Per-Revolution";
const std::string MouseParser::SENSOR_TAG = "Sensor";
const std::string MouseParser::RADIUS_TAG = "Radius";
const std::string MouseParser::RANGE_TAG = "Range";
const std::string MouseParser::HALF_WIDTH_TAG = "Half-Width";
const std::string MouseParser::READ_DURATION_TAG = "Read-Duration";

MouseParser::MouseParser(const std::string& filePath, bool* success) :
        m_forwardDirection(Radians(0)),
        m_centerOfMass(Cartesian(Meters(0), Meters(0))) {
    pugi::xml_parse_result result = m_doc.load_file(filePath.c_str());
    if (!result) {
        L()->warn(
            "Unable to read mouse parameters in \"%v\" - %v",
            filePath, result.description());
        *success = false;
    }
    else {
        // TODO: MACK - what if this fails...
        m_forwardDirection = getForwardDirection();
        m_centerOfMass = getCenterOfMass();
    }
}

Polygon MouseParser::getBody(
        const Cartesian& initialTranslation, const Radians& initialRotation, bool* success) {

    Cartesian alignmentTranslation = initialTranslation - m_centerOfMass;
    Radians alignmentRotation = initialRotation - m_forwardDirection;

    pugi::xml_node body = m_doc.child(BODY_TAG.c_str());
    if (body.begin() == body.end()) {
        L()->warn("No \"%v\" tag found.", BODY_TAG);
        *success = false;
        return NULL_POLYGON;
    }

    std::vector<Cartesian> vertices;
    for (auto it = body.begin(); it != body.end(); ++it) {
        pugi::xml_node p = *it;
        double x = getDoubleIfHasDouble(p, X_TAG, success);
        double y = getDoubleIfHasDouble(p, Y_TAG, success);
        if (!*success) {
            return NULL_POLYGON;
        }
        vertices.push_back(
            alignVertex(
                Cartesian(Meters(x), Meters(y)),
                alignmentTranslation,
                alignmentRotation,
                initialTranslation));
    }

    if (vertices.size() < 3) {
        L()->warn(
            "Invalid mouse \"%v\" - less than three vertices were specified.",
            BODY_TAG);
        *success = false;
        return NULL_POLYGON;
    }

    return Polygon(vertices);
}

std::map<std::string, Wheel> MouseParser::getWheels(
        const Cartesian& initialTranslation, const Radians& initialRotation, bool* success) {

    Cartesian alignmentTranslation = initialTranslation - getCenterOfMass();
    Radians alignmentRotation = initialRotation - getForwardDirection();

    std::map<std::string, Wheel> wheels;
    for (pugi::xml_node wheel : m_doc.children(WHEEL_TAG.c_str())) {

        std::string name = wheel.child(NAME_TAG.c_str()).child_value();
        if (name.empty()) {
            L()->warn("No wheel name specified.");
            *success = false;
            break;
        }
        if (SimUtilities::mapContains(wheels, name)) {
            L()->warn("Two wheels both have the name \"%v\".", name);
            *success = false;
            break;
        }

        double diameter = getDoubleIfHasDouble(wheel, DIAMETER_TAG, success);
        double width = getDoubleIfHasDouble(wheel, WIDTH_TAG, success);

        pugi::xml_node position = wheel.child(POSITION_TAG.c_str());
        if (!position) {
            L()->warn(
                "No wheel \"%v\" tag found. This means that the \"%v\" and"
                " \"%v\" tags won't be found either.",
                POSITION_TAG, X_TAG, Y_TAG);
            *success = false;
        }
        double x = getDoubleIfHasDouble(position, X_TAG, success);
        double y = getDoubleIfHasDouble(position, Y_TAG, success);

        double direction = getDoubleIfHasDouble(wheel, DIRECTION_TAG, success);
        double maxAngularVelocityMagnitude = getDoubleIfHasDouble(wheel, MAX_SPEED_TAG, success);

        EncoderType encoderType;
        std::string encoderTypeString = wheel.child(ENCODER_TYPE_TAG.c_str()).child_value();
        if (SimUtilities::mapContains(STRING_TO_ENCODER_TYPE, encoderTypeString)) {
            encoderType = STRING_TO_ENCODER_TYPE.at(encoderTypeString);
        }
        else {
            L()->warn(
                "The encoder type \"%v\" is not valid. The only valid encoder"
                " types are \"%v\" and \"%v\".",
                encoderTypeString, 
                ENCODER_TYPE_TO_STRING.at(EncoderType::ABSOLUTE),
                ENCODER_TYPE_TO_STRING.at(EncoderType::RELATIVE));
            *success = false;
        }

        double encoderTicksPerRevolution = getDoubleIfHasDouble(wheel, ENCODER_TICKS_PER_REVOLUTION_TAG, success);

        if (success) {
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
    }

    return wheels;
}

std::map<std::string, Sensor> MouseParser::getSensors(
        const Cartesian& initialTranslation, const Radians& initialRotation, bool* success) {

    Cartesian alignmentTranslation = initialTranslation - getCenterOfMass();
    Radians alignmentRotation = initialRotation - getForwardDirection();

    std::map<std::string, Sensor> sensors;
    for (pugi::xml_node sensor : m_doc.children(SENSOR_TAG.c_str())) {

        std::string name = sensor.child(NAME_TAG.c_str()).child_value();
        if (name.empty()) {
            L()->warn("No wheel name specified.");
            *success = false;
            break;
        }
        if (SimUtilities::mapContains(sensors, name)) {
            L()->warn("Two wheels both have the name \"%v\".", name);
            *success = false;
            break;
        }

        double radius = getDoubleIfHasDouble(sensor, RADIUS_TAG, success);
        double range = getDoubleIfHasDouble(sensor, RANGE_TAG, success);
        double halfWidth = getDoubleIfHasDouble(sensor, HALF_WIDTH_TAG, success);
        double readDuration = getDoubleIfHasDouble(sensor, READ_DURATION_TAG, success);

        pugi::xml_node position = sensor.child(POSITION_TAG.c_str());
        if (!position) {
            L()->warn(
                "No sensor \"%v\" tag found. This means that the \"%v\" and"
                " \"%v\" tags won't be found either.",
                POSITION_TAG, X_TAG, Y_TAG);
            *success = false;
        }
        double x = getDoubleIfHasDouble(position, X_TAG, success);
        double y = getDoubleIfHasDouble(position, Y_TAG, success);

        double direction = getDoubleIfHasDouble(sensor, DIRECTION_TAG, success);

        if (success) {
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
    }

    return sensors;
}

double MouseParser::getDoubleIfHasDouble(const pugi::xml_node& node, const std::string& tag, bool* success) {
    std::string valueString = node.child(tag.c_str()).child_value();
    if (!SimUtilities::isDouble(valueString)) {
        L()->warn(
            "Invalid value for tag \"%v\" - the tag is either missing entirely,"
            " or its value isn't a valid floating point number.", tag);
        *success = false;
        return 0.0;
    }
    return SimUtilities::strToDouble(valueString);
}

Cartesian MouseParser::alignVertex(const Cartesian& vertex, const Cartesian& alignmentTranslation,
        const Radians& alignmentRotation, const Cartesian& rotationPoint) {
    Cartesian translated = GeometryUtilities::translateVertex(vertex, alignmentTranslation);
    return GeometryUtilities::rotateVertexAroundPoint(translated, alignmentRotation, rotationPoint);
}

Radians MouseParser::getForwardDirection() {
    // TODO: MACK Handle the failing case
    double degrees = SimUtilities::strToDouble(
        m_doc.child("Forward-Direction").child_value());
    return Radians(Degrees(degrees));
}

Cartesian MouseParser::getCenterOfMass() {
    // TODO: MACK Handle the failing case
    std::vector<Cartesian> vertices;
    pugi::xml_node center = m_doc.child("Center-of-Mass");
    double x = SimUtilities::strToDouble(center.child("X").child_value());
    double y = SimUtilities::strToDouble(center.child("Y").child_value());
    return Cartesian(Meters(x), Meters(y));
}

} // namespace sim

#include "MouseParser.h"

#include "Assert.h"
#include "ContainerUtilities.h"
#include "EncoderType.h"
#include "GeometryUtilities.h"
#include "SimUtilities.h"
#include "units/RevolutionsPerMinute.h"

namespace sim {

const Polygon MouseParser::NULL_POLYGON = Polygon({
    Cartesian(Meters(0), Meters(0)),
    Cartesian(Meters(0), Meters(0)),
    Cartesian(Meters(0), Meters(0)),
});
const std::string MouseParser::MOUSE_TAG = "Mouse";
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
        m_root = m_doc.child(MOUSE_TAG.c_str());
        m_forwardDirection = Radians(Degrees(
            getDoubleIfHasDouble(m_root, FORWARD_DIRECTION_TAG, success)));
        pugi::xml_node centerOfMassNode = getContainerNode(m_root, CENTER_OF_MASS_TAG, success);
        double x = getDoubleIfHasDouble(centerOfMassNode, X_TAG, success);
        double y = getDoubleIfHasDouble(centerOfMassNode, Y_TAG, success);
        m_centerOfMass = Cartesian(Meters(x), Meters(y));
    }
}

Polygon MouseParser::getBody(
        const Cartesian& initialTranslation, const Radians& initialRotation, bool* success) {

    Cartesian alignmentTranslation = initialTranslation - m_centerOfMass;
    Radians alignmentRotation = initialRotation - m_forwardDirection;

    pugi::xml_node body = m_root.child(BODY_TAG.c_str());
    if (body.begin() == body.end()) {
        L()->warn("No \"%v\" tag found.", BODY_TAG);
        *success = false;
    }

    std::vector<Cartesian> vertices;
    for (pugi::xml_node vertex : body.children(VERTEX_TAG.c_str())) {
        double x = getDoubleIfHasDouble(vertex, X_TAG, success);
        double y = getDoubleIfHasDouble(vertex, Y_TAG, success);
        vertices.push_back(
            alignVertex(
                Cartesian(Meters(x), Meters(y)),
                alignmentTranslation,
                alignmentRotation,
                initialTranslation));
    }

    if (vertices.size() < 3) {
        L()->warn(
            "Invalid mouse \"%v\" - less than three valid vertices were specified.",
            BODY_TAG);
        *success = false;
    }

    Polygon bodyPolygon;
    if (success) {
        bodyPolygon = Polygon(vertices);
        if (bodyPolygon.getTriangles().size() == 0) {
            L()->warn(
                "Invalid mouse \"%v\" - the vertices specified do not"
                " constitute a simple polygon.", BODY_TAG);
            *success = false;
        }
    }

    if (!*success) {
        return NULL_POLYGON;
    }
    return bodyPolygon;
}

std::map<std::string, Wheel> MouseParser::getWheels(
        const Cartesian& initialTranslation, const Radians& initialRotation, bool* success) {

    Cartesian alignmentTranslation = initialTranslation - m_centerOfMass;
    Radians alignmentRotation = initialRotation - m_forwardDirection;

    std::map<std::string, Wheel> wheels;
    for (pugi::xml_node wheel : m_root.children(WHEEL_TAG.c_str())) {

        std::string name = getNameIfNonemptyAndUnique("wheel", wheel, wheels, success);
        double diameter = getDoubleIfHasDoubleAndNonNegative(wheel, DIAMETER_TAG, success);
        double width = getDoubleIfHasDoubleAndNonNegative(wheel, WIDTH_TAG, success);
        pugi::xml_node position = getContainerNode(wheel, POSITION_TAG, success);
        double x = getDoubleIfHasDouble(position, X_TAG, success);
        double y = getDoubleIfHasDouble(position, Y_TAG, success);
        double direction = getDoubleIfHasDouble(wheel, DIRECTION_TAG, success);
        double maxAngularVelocityMagnitude = getDoubleIfHasDoubleAndNonNegative(
            wheel, MAX_SPEED_TAG, success);
        EncoderType encoderType = getEncoderTypeIfValid(wheel, success);
        double encoderTicksPerRevolution = getDoubleIfHasDoubleAndNonNegative(
            wheel, ENCODER_TICKS_PER_REVOLUTION_TAG, success);

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
        const Cartesian& initialTranslation, const Radians& initialRotation, const Maze& maze, bool* success) {

    Cartesian alignmentTranslation = initialTranslation - m_centerOfMass;
    Radians alignmentRotation = initialRotation - m_forwardDirection;

    std::map<std::string, Sensor> sensors;
    for (pugi::xml_node sensor : m_root.children(SENSOR_TAG.c_str())) {

        std::string name = getNameIfNonemptyAndUnique("sensor", sensor, sensors, success);
        double radius = getDoubleIfHasDoubleAndNonNegative(sensor, RADIUS_TAG, success);
        double range = getDoubleIfHasDoubleAndNonNegative(sensor, RANGE_TAG, success);
        double halfWidth = getDoubleIfHasDoubleAndNonNegative(sensor, HALF_WIDTH_TAG, success);
        pugi::xml_node position = getContainerNode(sensor, POSITION_TAG, success);
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
                        alignVertex(
                            Cartesian(Meters(x), Meters(y)),
                            alignmentTranslation,
                            alignmentRotation,
                            initialTranslation),
                        Degrees(direction) + alignmentRotation,
                        maze)));
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

double MouseParser::getDoubleIfHasDoubleAndNonNegative(
        const pugi::xml_node& node, const std::string& tag, bool* success) {
    double value = getDoubleIfHasDouble(node, tag, success);
    if (value < 0.0) {
        L()->warn(
            "The value for tag \"%v\" is %v, which is less than the minimum"
            " allowed value of %v.", tag, value, 0.0);
        *success = false;
        return 0.0;
    }
    return value;
}

pugi::xml_node MouseParser::getContainerNode(const pugi::xml_node& node, const std::string& tag, bool* success) {
    pugi::xml_node containerNode = node.child(tag.c_str());
    if (!containerNode) {
        L()->warn(
            "No wheel \"%v\" tag found. This means that the \"%v\" and"
            " \"%v\" tags won't be found either.",
            tag, X_TAG, Y_TAG);
        *success = false;
    }
    return containerNode;
}

EncoderType MouseParser::getEncoderTypeIfValid(const pugi::xml_node& node, bool* success) {
    EncoderType encoderType;
    std::string encoderTypeString = node.child(ENCODER_TYPE_TAG.c_str()).child_value();
    if (ContainerUtilities::mapContains(STRING_TO_ENCODER_TYPE, encoderTypeString)) {
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
    return encoderType;
}

Cartesian MouseParser::alignVertex(const Cartesian& vertex, const Cartesian& alignmentTranslation,
        const Radians& alignmentRotation, const Cartesian& rotationPoint) {
    Cartesian translated = GeometryUtilities::translateVertex(vertex, alignmentTranslation);
    return GeometryUtilities::rotateVertexAroundPoint(translated, alignmentRotation, rotationPoint);
}

} // namespace sim

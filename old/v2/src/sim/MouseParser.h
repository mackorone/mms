#pragma once

#include <pugixml/pugixml.hpp>
#include <string>
#include <vector>

#include "ContainerUtilities.h"
#include "Logging.h"
#include "Maze.h"
#include "Polygon.h"
#include "Sensor.h"
#include "units/Cartesian.h"
#include "units/Meters.h"
#include "Wheel.h"

namespace sim {

class MouseParser {

public:
    MouseParser(const std::string& filePath, bool* success);
    Polygon getBody(
        const Cartesian& initialTranslation, const Radians& initialRotation, bool* success);
    std::map<std::string, Wheel> getWheels(
        const Cartesian& initialTranslation, const Radians& initialRotation, bool* success);
    std::map<std::string, Sensor> getSensors(
        const Cartesian& initialTranslation, const Radians& initialRotation, const Maze& maze, bool* success);

private:
    // We have to keep m_doc around so valgrind doesn't complain
    pugi::xml_document m_doc;
    pugi::xml_node m_root;
    Radians m_forwardDirection;
    Cartesian m_centerOfMass;

    double getDoubleIfHasDouble(const pugi::xml_node& node, const std::string& tag, bool* success);
    double getDoubleIfHasDoubleAndNonNegative(
        const pugi::xml_node& node, const std::string& tag, bool* success);
    pugi::xml_node getContainerNode(const pugi::xml_node& node, const std::string& tag, bool* success);
    EncoderType getEncoderTypeIfValid(const pugi::xml_node& node, bool* success);

    Cartesian alignVertex(const Cartesian& vertex, const Cartesian& alignmentTranslation,
        const Radians& alignmentRotation, const Cartesian& rotationPoint);

    static const Polygon NULL_POLYGON;
    static const std::string MOUSE_TAG;
    static const std::string FORWARD_DIRECTION_TAG;
    static const std::string CENTER_OF_MASS_TAG;
    static const std::string BODY_TAG;
    static const std::string VERTEX_TAG;
    static const std::string X_TAG;
    static const std::string Y_TAG;
    static const std::string WHEEL_TAG;
    static const std::string NAME_TAG;
    static const std::string DIAMETER_TAG;
    static const std::string WIDTH_TAG;
    static const std::string POSITION_TAG;
    static const std::string DIRECTION_TAG;
    static const std::string MAX_SPEED_TAG;
    static const std::string ENCODER_TYPE_TAG;
    static const std::string ENCODER_TICKS_PER_REVOLUTION_TAG;
    static const std::string SENSOR_TAG;
    static const std::string RADIUS_TAG;
    static const std::string RANGE_TAG;
    static const std::string HALF_WIDTH_TAG;

    template<class T>
    std::string getNameIfNonemptyAndUnique(const std::string& type,
            const pugi::xml_node& node, const std::map<std::string, T>& map, bool* success) {
        std::string name = node.child(NAME_TAG.c_str()).child_value();
        if (name.empty()) {
            L()->warn("No %v name specified.", type);
            *success = false;
        }
        if (ContainerUtilities::mapContains(map, name)) {
            L()->warn("Two %vs both have the name \"%v\".", type, name);
            *success = false;
        }
        return name;
    }
};

} // namespace sim

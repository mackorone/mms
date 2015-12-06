#pragma once

#include <pugixml/pugixml.hpp>
#include <string>
#include <vector>

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
        const Cartesian& initialTranslation, const Radians& initialRotation, bool* success);

private:
    pugi::xml_document m_doc;
    Radians m_forwardDirection;
    Cartesian m_centerOfMass;

    // TODO: MACK - kill these
    Radians getForwardDirection();
    Cartesian getCenterOfMass();

    double getDoubleIfHasDouble(const pugi::xml_node& node, const std::string& tag, bool* success);
    Cartesian alignVertex(const Cartesian& vertex, const Cartesian& alignmentTranslation,
        const Radians& alignmentRotation, const Cartesian& rotationPoint);

    static const Polygon NULL_POLYGON;
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
    static const std::string READ_DURATION_TAG;
};

} // namespace sim

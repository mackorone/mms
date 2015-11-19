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

// TODO: MACK - the important thing is to decouple the specification in the mouse file with the mouse in the simulator

public:
    MouseParser(const std::string& filePath);
    Radians getForwardDirection(); // TODO: MACK - Kill this
    Cartesian getCenterOfMass(); // TODO: MACK - Kill this
    Polygon getBody(
        const Cartesian& initialTranslation, const Radians& initialRotation);
    std::map<std::string, Wheel> getWheels(
        const Cartesian& initialTranslation, const Radians& initialRotation);
    std::map<std::string, Sensor> getSensors(
        const Cartesian& initialTranslation, const Radians& initialRotation);

private:
    pugi::xml_document m_doc;

    Cartesian alignVertex(const Cartesian& vertex, const Cartesian& alignmentTranslation,
        const Radians& alignmentRotation, const Cartesian& rotationPoint);
};

} // namespace sim

#pragma once

#include <map>

#include "ContainerUtilities.h"

namespace sim {

enum class SensorType { ANALOG, DIGITAL };

static const std::map<SensorType, std::string> SENSOR_TYPE_TO_STRING {
    {SensorType::ANALOG, "ANALOG"},
    {SensorType::DIGITAL, "DIGITAL"},
};

static const std::map<std::string, SensorType> STRING_TO_SENSOR_TYPE =
    ContainerUtilities::inverse(SENSOR_TYPE_TO_STRING);

} // namespace sim

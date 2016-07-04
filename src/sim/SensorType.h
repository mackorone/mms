#pragma once

#include <map>

#include "ContainerUtilities.h"

namespace sim {

enum class SensorType {
    ANALOG,
    DIGITAL // TODO: MACK - comma here
};

static const QMap<SensorType, std::string> SENSOR_TYPE_TO_STRING {
    {SensorType::ANALOG, "ANALOG"},
    {SensorType::DIGITAL, "DIGITAL"},
};

static const QMap<std::string, SensorType> STRING_TO_SENSOR_TYPE =
    ContainerUtilities::inverse(SENSOR_TYPE_TO_STRING);

} // namespace sim

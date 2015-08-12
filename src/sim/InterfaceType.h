#pragma once

#include <map>

namespace sim {

enum class InterfaceType { DISCRETE, CONTINUOUS };

static const std::map<InterfaceType, std::string> INTERFACE_TYPE_TO_STRING {
    {InterfaceType::DISCRETE, "DISCRETE"},
    {InterfaceType::CONTINUOUS, "CONTINUOUS"},
};

static const std::map<std::string, InterfaceType> STRING_TO_INTERFACE_TYPE {
    {"DISCRETE", InterfaceType::DISCRETE},
    {"CONTINUOUS", InterfaceType::CONTINUOUS},
};

} // namespace sim

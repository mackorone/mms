#pragma once

#include <map>
#include <vector>

namespace sim {

enum class LayoutType { FULL, ZOOMED, BOTH };

static const std::map<LayoutType, LayoutType> LAYOUT_TYPE_CYCLE {
    {LayoutType::FULL, LayoutType::ZOOMED},
    {LayoutType::ZOOMED, LayoutType::BOTH},
    {LayoutType::BOTH, LayoutType::FULL},
};

static const std::map<LayoutType, std::string> LAYOUT_TYPE_TO_STRING {
    {LayoutType::FULL, "FULL"},
    {LayoutType::ZOOMED, "ZOOMED"},
    {LayoutType::BOTH, "BOTH"},
};

static const std::map<std::string, LayoutType> STRING_TO_LAYOUT_TYPE {
    {"FULL", LayoutType::FULL},
    {"ZOOMED", LayoutType::ZOOMED},
    {"BOTH", LayoutType::BOTH},
};

} // namespace sim

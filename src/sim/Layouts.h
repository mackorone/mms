#pragma once

#include <map>
#include <vector>

namespace sim {

enum class Layout { FULL, ZOOMED, BOTH };

static const std::map<Layout, Layout> LAYOUT_CYCLE {
    {Layout::FULL, Layout::ZOOMED},
    {Layout::ZOOMED, Layout::BOTH},
    {Layout::BOTH, Layout::FULL},
};

static const std::map<std::string, Layout> STRING_TO_LAYOUT {
    {"FULL", Layout::FULL},
    {"ZOOMED", Layout::ZOOMED},
    {"BOTH", Layout::BOTH},
};

} // namespace sim

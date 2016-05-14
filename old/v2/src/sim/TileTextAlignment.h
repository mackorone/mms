#pragma once

#include <map>
#include <set>

#include "ContainerUtilities.h"

namespace sim {

enum class TileTextAlignment {
    UPPER_LEFT, UPPER_CENTER, UPPER_RIGHT,
    CENTER_LEFT, CENTER_CENTER, CENTER_RIGHT,
    LOWER_LEFT, LOWER_CENTER, LOWER_RIGHT
};

static const std::map<TileTextAlignment, std::string> TILE_TEXT_ALIGNMENT_TO_STRING {
    {TileTextAlignment::UPPER_LEFT, "UPPER_LEFT"},
    {TileTextAlignment::UPPER_CENTER, "UPPER_CENTER"},
    {TileTextAlignment::UPPER_RIGHT, "UPPER_RIGHT"},
    {TileTextAlignment::CENTER_LEFT, "CENTER_LEFT"},
    {TileTextAlignment::CENTER_CENTER, "CENTER_CENTER"},
    {TileTextAlignment::CENTER_RIGHT, "CENTER_RIGHT"},
    {TileTextAlignment::LOWER_LEFT, "LOWER_LEFT"},
    {TileTextAlignment::LOWER_CENTER, "LOWER_CENTER"},
    {TileTextAlignment::LOWER_RIGHT, "LOWER_RIGHT"},
};

static const std::map<std::string, TileTextAlignment> STRING_TO_TILE_TEXT_ALIGNMENT =
    ContainerUtilities::inverse(TILE_TEXT_ALIGNMENT_TO_STRING);

static const std::set<TileTextAlignment> UPPER_STAR_ALIGNMENTS {
    TileTextAlignment::UPPER_LEFT, 
    TileTextAlignment::UPPER_CENTER, 
    TileTextAlignment::UPPER_RIGHT, 
};

static const std::set<TileTextAlignment> CENTER_STAR_ALIGNMENTS {
    TileTextAlignment::CENTER_LEFT, 
    TileTextAlignment::CENTER_CENTER, 
    TileTextAlignment::CENTER_RIGHT, 
};

static const std::set<TileTextAlignment> LOWER_STAR_ALIGNMENTS {
    TileTextAlignment::LOWER_LEFT, 
    TileTextAlignment::LOWER_CENTER, 
    TileTextAlignment::LOWER_RIGHT, 
};

static const std::set<TileTextAlignment> STAR_LEFT_ALIGNMENTS {
    TileTextAlignment::UPPER_LEFT, 
    TileTextAlignment::CENTER_LEFT, 
    TileTextAlignment::LOWER_LEFT, 
};

static const std::set<TileTextAlignment> STAR_CENTER_ALIGNMENTS {
    TileTextAlignment::UPPER_CENTER, 
    TileTextAlignment::CENTER_CENTER, 
    TileTextAlignment::LOWER_CENTER, 
};

static const std::set<TileTextAlignment> STAR_RIGHT_ALIGNMENTS {
    TileTextAlignment::UPPER_RIGHT, 
    TileTextAlignment::CENTER_RIGHT, 
    TileTextAlignment::LOWER_RIGHT, 
};

} // namespace sim

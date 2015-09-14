#pragma once

#include <map>
#include <tuple>

namespace sim {

enum class Color {
    BLACK,
    BLUE,
    CYAN,
    GRAY,
    GREEN,
    ORANGE,
    RED,
    WHITE,
    YELLOW,
    DARK_BLUE,
    DARK_CYAN,
    DARK_GRAY,
    DARK_GREEN,
    DARK_RED,
    DARK_YELLOW,
};

// TODO: MACK 3 - Can these be doubles
static const std::map<Color, std::tuple<float, float, float>> COLOR_TO_RGB {
    {Color::BLACK      , std::make_tuple(0.0f, 0.0f, 0.0f)},
    {Color::BLUE       , std::make_tuple(0.0f, 0.0f, 0.7f)},
    {Color::CYAN       , std::make_tuple(0.0f, 0.4f, 0.4f)},
    {Color::GRAY       , std::make_tuple(0.7f, 0.7f, 0.7f)},
    {Color::GREEN      , std::make_tuple(0.0f, 0.7f, 0.0f)},
    {Color::ORANGE     , std::make_tuple(0.7f, 0.4f, 0.0f)},
    {Color::RED        , std::make_tuple(0.8f, 0.0f, 0.0f)},
    {Color::WHITE      , std::make_tuple(1.0f, 1.0f, 1.0f)},
    {Color::YELLOW     , std::make_tuple(0.7f, 0.7f, 0.0f)},
    {Color::DARK_BLUE  , std::make_tuple(0.0f, 0.0f, 0.2f)},
    {Color::DARK_CYAN  , std::make_tuple(0.0f, 0.2f, 0.2f)},
    {Color::DARK_GRAY  , std::make_tuple(0.1f, 0.1f, 0.1f)},
    {Color::DARK_GREEN , std::make_tuple(0.0f, 0.3f, 0.0f)},
    {Color::DARK_RED   , std::make_tuple(0.3f, 0.0f, 0.0f)},
    {Color::DARK_YELLOW, std::make_tuple(0.1f, 0.1f, 0.0f)},
};

static const std::map<Color, std::string> COLOR_TO_STRING = {
    {Color::BLACK      , "BLACK"      },
    {Color::BLUE       , "BLUE"       },
    {Color::CYAN       , "CYAN"       },
    {Color::GRAY       , "GRAY"       },
    {Color::GREEN      , "GREEN"      },
    {Color::ORANGE     , "ORANGE"     },
    {Color::RED        , "RED"        },
    {Color::WHITE      , "WHITE"      },
    {Color::YELLOW     , "YELLOW"     },
    {Color::DARK_BLUE  , "DARK_BLUE"  },
    {Color::DARK_CYAN  , "DARK_CYAN"  },
    {Color::DARK_GRAY  , "DARK_GRAY"  },
    {Color::DARK_GREEN , "DARK_GREEN" },
    {Color::DARK_RED   , "DARK_RED"   },
    {Color::DARK_YELLOW, "DARK_YELLOW"},
};

static const std::map<std::string, Color> STRING_TO_COLOR = {
    {"BLACK"      , Color::BLACK      },
    {"BLUE"       , Color::BLUE       },
    {"CYAN"       , Color::CYAN       },
    {"GRAY"       , Color::GRAY       },
    {"GREEN"      , Color::GREEN      },
    {"ORANGE"     , Color::ORANGE     },
    {"RED"        , Color::RED        },
    {"WHITE"      , Color::WHITE      },
    {"YELLOW"     , Color::YELLOW     },
    {"DARK_BLUE"  , Color::DARK_BLUE  },
    {"DARK_CYAN"  , Color::DARK_CYAN  },
    {"DARK_GRAY"  , Color::DARK_GRAY  },
    {"DARK_GREEN" , Color::DARK_GREEN },
    {"DARK_RED"   , Color::DARK_RED   },
    {"DARK_YELLOW", Color::DARK_YELLOW},
};

static const std::map<char, Color> CHAR_TO_COLOR = {
    {'k', Color::BLACK      },
    {'b', Color::BLUE       },
    {'a', Color::GRAY       },
    {'c', Color::CYAN       },
    {'g', Color::GREEN      },
    {'o', Color::ORANGE     },
    {'r', Color::RED        },
    {'w', Color::WHITE      },
    {'y', Color::YELLOW     },
    {'B', Color::DARK_BLUE  },
    {'C', Color::DARK_CYAN  },
    {'A', Color::DARK_GRAY  },
    {'G', Color::DARK_GREEN },
    {'R', Color::DARK_RED   },
    {'Y', Color::DARK_YELLOW},
};

} // namespace sim

#pragma once

#include <map>
#include <tuple>

#include "ContainerUtilities.h"

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
    DARK_ORANGE,
    DARK_RED,
    DARK_VIOLET,
    DARK_YELLOW,
};

static const std::map<Color, std::tuple<double, double, double>> COLOR_TO_RGB {
    {Color::BLACK      , std::make_tuple(0.0, 0.0, 0.0)},
    {Color::BLUE       , std::make_tuple(0.0, 0.0, 0.7)},
    {Color::CYAN       , std::make_tuple(0.0, 0.4, 0.4)},
    {Color::GRAY       , std::make_tuple(0.7, 0.7, 0.7)},
    {Color::GREEN      , std::make_tuple(0.0, 0.7, 0.0)},
    {Color::ORANGE     , std::make_tuple(0.7, 0.4, 0.0)},
    {Color::RED        , std::make_tuple(0.8, 0.0, 0.0)},
    {Color::WHITE      , std::make_tuple(1.0, 1.0, 1.0)},
    {Color::YELLOW     , std::make_tuple(0.7, 0.7, 0.0)},
    {Color::DARK_BLUE  , std::make_tuple(0.0, 0.0, 0.2)},
    {Color::DARK_CYAN  , std::make_tuple(0.0, 0.2, 0.2)},
    {Color::DARK_GRAY  , std::make_tuple(0.1, 0.1, 0.1)},
    {Color::DARK_GREEN , std::make_tuple(0.0, 0.3, 0.0)},
    {Color::DARK_ORANGE, std::make_tuple(0.2, 0.1, 0.0)},
    {Color::DARK_RED   , std::make_tuple(0.3, 0.0, 0.0)},
    {Color::DARK_VIOLET, std::make_tuple(0.2, 0.0, 0.2)},
    {Color::DARK_YELLOW, std::make_tuple(0.2, 0.2, 0.0)},
};

static const std::map<Color, std::string> COLOR_TO_STRING {
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
    {Color::DARK_ORANGE, "DARK_ORANGE"},
    {Color::DARK_RED   , "DARK_RED"   },
    {Color::DARK_VIOLET, "DARK_VIOLET"},
    {Color::DARK_YELLOW, "DARK_YELLOW"},
};

static const std::map<std::string, Color> STRING_TO_COLOR =
    ContainerUtilities::inverse(COLOR_TO_STRING);

static const std::map<Color, char> COLOR_TO_CHAR {
    {Color::BLACK      , 'k'},
    {Color::BLUE       , 'b'},
    {Color::GRAY       , 'a'},
    {Color::CYAN       , 'c'},
    {Color::GREEN      , 'g'},
    {Color::ORANGE     , 'o'},
    {Color::RED        , 'r'},
    {Color::WHITE      , 'w'},
    {Color::YELLOW     , 'y'},
    {Color::DARK_BLUE  , 'B'},
    {Color::DARK_CYAN  , 'C'},
    {Color::DARK_GRAY  , 'A'},
    {Color::DARK_GREEN , 'G'},
    {Color::DARK_ORANGE, 'O'},
    {Color::DARK_RED   , 'R'},
    {Color::DARK_VIOLET, 'V'},
    {Color::DARK_YELLOW, 'Y'},
};

static const std::map<char, Color> CHAR_TO_COLOR =
    ContainerUtilities::inverse(COLOR_TO_CHAR);

} // namespace sim

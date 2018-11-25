#include "Color.h"

#include "ContainerUtilities.h"

namespace mms {

const QMap<Color, RGB>& COLOR_TO_RGB() {
    static const QMap<Color, RGB> map = {
        {Color::BLACK      , {0.0, 0.0, 0.0}},
        {Color::BLUE       , {0.0, 0.0, 0.7}},
        {Color::CYAN       , {0.0, 0.4, 0.4}},
        {Color::GRAY       , {0.7, 0.7, 0.7}},
        {Color::GREEN      , {0.0, 0.7, 0.0}},
        {Color::ORANGE     , {0.7, 0.4, 0.0}},
        {Color::RED        , {0.8, 0.0, 0.0}},
        {Color::WHITE      , {1.0, 1.0, 1.0}},
        {Color::YELLOW     , {0.7, 0.7, 0.0}},
        {Color::DARK_BLUE  , {0.0, 0.0, 0.2}},
        {Color::DARK_CYAN  , {0.0, 0.2, 0.2}},
        {Color::DARK_GRAY  , {0.1, 0.1, 0.1}},
        {Color::DARK_GREEN , {0.0, 0.3, 0.0}},
        {Color::DARK_ORANGE, {0.2, 0.1, 0.0}},
        {Color::DARK_RED   , {0.3, 0.0, 0.0}},
        {Color::DARK_VIOLET, {0.2, 0.0, 0.2}},
        {Color::DARK_YELLOW, {0.2, 0.2, 0.0}},
    };
    return map;
}

const QMap<Color, QString>& COLOR_TO_STRING() {
    static const QMap<Color, QString> map = {
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
    return map;
}

const QMap<QString, Color>& STRING_TO_COLOR() {
    static const QMap<QString, Color> map =
        ContainerUtilities::inverse(COLOR_TO_STRING());
    return map;
}

const QMap<Color, QChar>& COLOR_TO_CHAR() {
    static const QMap<Color, QChar> map = {
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
    return map;
}

const QMap<QChar, Color>& CHAR_TO_COLOR() {
    static const QMap<QChar, Color> map =
        ContainerUtilities::inverse(COLOR_TO_CHAR());
    return map;
}

} 

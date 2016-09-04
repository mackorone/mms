#pragma once

#include <iostream>

#define PRINT_IF_NONEMPTY(arg) {\
    if (arg != "") {\
        std::cerr << " ";\
        std::cerr << arg;\
    }\
}

#define PRINT(FNAME, ...) {\
    std::cerr << FNAME;\
    PRINT_IF_NONEMPTY(VARARGS);\
    std::cerr << std::endl;\
}

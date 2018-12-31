#pragma once

// NOTE: Only supports up to four arguments for now (truncates additional args)

#include <iostream>

#define GET_1ST_ARG(FNAME, _1, ...) _1
#define GET_2ND_ARG(FNAME, _1, _2, ...) _2
#define GET_3RD_ARG(FNAME, _1, _2, _3, ...) _3
#define GET_4TH_ARG(FNAME, _1, _2, _3, _4, ...) _4

#define GET_6TH_ELEMENT(_1, _2, _3, _4, _5, _6, ...) _6
#define HAS_1ST_ARG(...) GET_6TH_ELEMENT(__VA_ARGS__, true,  true,  true,  true, false)
#define HAS_2ND_ARG(...) GET_6TH_ELEMENT(__VA_ARGS__, true,  true,  true, false, false)
#define HAS_3RD_ARG(...) GET_6TH_ELEMENT(__VA_ARGS__, true,  true, false, false, false)
#define HAS_4TH_ARG(...) GET_6TH_ELEMENT(__VA_ARGS__, true, false, false, false, false)

#define PRINT_FNAME(FNAME, ...) {\
    std::cout << FNAME;\
};

#define PRINT_ARG(HAS_FUNC, GET_FUNC, ...) {\
    if (HAS_FUNC(__VA_ARGS__)) {\
        std::cout << " ";\
        std::cout << GET_FUNC(__VA_ARGS__, "", "", "", "");\
    }\
}

#define PRINT(...) {\
    PRINT_FNAME(__VA_ARGS__);\
    PRINT_ARG(HAS_1ST_ARG, GET_1ST_ARG, __VA_ARGS__);\
    PRINT_ARG(HAS_2ND_ARG, GET_2ND_ARG, __VA_ARGS__);\
    PRINT_ARG(HAS_3RD_ARG, GET_3RD_ARG, __VA_ARGS__);\
    PRINT_ARG(HAS_4TH_ARG, GET_4TH_ARG, __VA_ARGS__);\
    std::cout << std::endl;\
}

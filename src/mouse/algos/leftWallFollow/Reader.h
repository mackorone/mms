#pragma once

#include <cstdlib>
#include <string>

#define READ()\
std::string input;\
std::cin >> input;\
if (input == "crash" || input == "invalid") {\
    throw;\
}

#define READ_AND_RETURN_BOOL() {\
    READ();\
    return input == "true";\
}

#define READ_AND_RETURN_CHAR() {\
    READ();\
    return input.at(0);\
}

#define READ_AND_RETURN_DOUBLE() {\
    READ();\
    return atof(input.c_str());\
}

#define READ_AND_RETURN_INT() {\
    READ();\
    return atoi(input.c_str());\
}

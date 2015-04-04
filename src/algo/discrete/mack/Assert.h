#pragma once

// 0 = off
// 1 = on
#if (1)

#include <iostream>

#define ASSERT(condition) \
if ( !(condition) ) { \
    std::cout << "---------- Assertion failed! ----------" << std::endl \
              << "FILE: " << __FILE__   << std::endl \
              << "LINE: " << __LINE__   << std::endl \
              << "COND: " << #condition << std::endl \
              << "---------------------------------------" << std::endl; \
    exit(1); \
}

#else

#define ASSERT(condition) ;

#endif

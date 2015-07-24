#pragma once

#include "Options.h"

#if (SIMULATOR)

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

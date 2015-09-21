#pragma once

// ASSERT is a simple, development-time condition-checking mechanism. It is
// meant to be used by authors of mms code and, as such, simply prints to
// std::cerr. If, for some reason, this is insufficient for you, you're
// probably using it incorrectly. 

#include "CompiledParams.h"
#if (ASSERTS_ON)

#include <iostream>

// TODO: upforgrabs
// Get rid of ASSERT(condition) and add the following:
// - ASSERT_TRUE
// - ASSERT_FALSE
// - ASSERT_EQUAL (already done for you)
// - ASSERT_NOT_EQUAL
// - ASSERT_LESS_THAN
// Then search the code for every use of ASSERT(condition) and replace it with
// the appropriate new assert. If there are places that require
// ASSERT_LESS_THAN_OR_EQUAL, write it and use it. If not, don't.

#define ASSERT(condition)\
if (!(condition)) {\
    std::cerr << "---------- Assertion failed! ----------" << std::endl\
              << "FILE: " <<  __FILE__  << std::endl\
              << "LINE: " <<  __LINE__  << std::endl\
              << "COND: " << #condition << std::endl\
              << "---------------------------------------" << std::endl;\
    exit(1);\
}

#define ASSERT_EQUAL(one, two)\
if ((one) != (two)) {\
    std::cerr << "---------- Assertion failed! ----------" << std::endl\
              << "FILE: " <<  __FILE__  << std::endl\
              << "LINE: " <<  __LINE__  << std::endl\
              << "COND: " << #one << " == " << #two << std::endl\
              << "LHS: " << one << std::endl\
              << "RHS: " << two << std::endl\
              << "---------------------------------------" << std::endl;\
    exit(1);\
}

#else // if (ASSERTS_ON)

#define ASSERT(condition) ;
#define ASSERT_EQUAL(one, two) ;

#endif // if (ASSERTS_ON)

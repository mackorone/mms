#pragma once

#include "Options.h"

#if (SIMULATOR)

    #include <iostream>
    #include <string>

    #define OPENING "---------- Assertion failed! ----------"
    #define CLOSING "---------------------------------------"

    #define ASSERT_TR(condition)\
    if (!(condition)) {\
        std::cerr << OPENING << std::endl\
                  << "FILE: " <<  __FILE__  << std::endl\
                  << "LINE: " <<  __LINE__  << std::endl\
                  << "COND: " << #condition << std::endl\
                  << CLOSING << std::endl;\
        exit(1);\
    }

    #define ASSERT_FA(condition)\
    if (condition) {\
        std::cerr << OPENING << std::endl\
                  << "FILE: " <<  __FILE__  << std::endl\
                  << "LINE: " <<  __LINE__  << std::endl\
                  << "COND: " << "!(" << #condition << ")" << std::endl\
                  << CLOSING << std::endl;\
        exit(1);\
    }

    #define ASSERT_EQ(lhs, rhs)\
    if (!((lhs) == (rhs))) {\
        std::cerr << OPENING << std::endl\
                  << "FILE: " <<  __FILE__  << std::endl\
                  << "LINE: " <<  __LINE__  << std::endl\
                  << "COND: " << #lhs << " == " << #rhs << std::endl\
                  << "LHS: " << std::to_string(lhs) << std::endl\
                  << "RHS: " << std::to_string(rhs) << std::endl\
                  << CLOSING << std::endl;\
        exit(1);\
    }

    #define ASSERT_NE(lhs, rhs)\
    if (!((lhs) != (rhs))) {\
        std::cerr << OPENING << std::endl\
                  << "FILE: " <<  __FILE__  << std::endl\
                  << "LINE: " <<  __LINE__  << std::endl\
                  << "COND: " << #lhs << " != " << #rhs << std::endl\
                  << "LHS: " << std::to_string(lhs) << std::endl\
                  << "RHS: " << std::to_string(rhs) << std::endl\
                  << CLOSING << std::endl;\
        exit(1);\
    }

    #define ASSERT_LT(lhs, rhs)\
    if (!((lhs) < (rhs))) {\
        std::cerr << OPENING << std::endl\
                  << "FILE: " <<  __FILE__  << std::endl\
                  << "LINE: " <<  __LINE__  << std::endl\
                  << "COND: " << #lhs << " < " << #rhs << std::endl\
                  << "LHS: " << std::to_string(lhs) << std::endl\
                  << "RHS: " << std::to_string(rhs) << std::endl\
                  << CLOSING << std::endl;\
        exit(1);\
    }

    #define ASSERT_LE(lhs, rhs)\
    if (!((lhs) <= (rhs))) {\
        std::cerr << OPENING << std::endl\
                  << "FILE: " <<  __FILE__  << std::endl\
                  << "LINE: " <<  __LINE__  << std::endl\
                  << "COND: " << #lhs << " <= " << #rhs << std::endl\
                  << "LHS: " << std::to_string(lhs) << std::endl\
                  << "RHS: " << std::to_string(rhs) << std::endl\
                  << CLOSING << std::endl;\
        exit(1);\
    }

#else

    #define ASSERT_TR(condition) ;
    #define ASSERT_FA(condition) ;
    #define ASSERT_EQ(lhs, rhs) ;
    #define ASSERT_NE(lhs, rhs) ;
    #define ASSERT_LT(lhs, rhs) ;
    #define ASSERT_LE(lhs, rhs) ;

#endif
